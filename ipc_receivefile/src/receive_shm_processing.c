/*
 * receive_shm_processing.c
 *
 *  Created on: Feb 25, 2022
 *      Author: maksim
 */

#include "receive_shm_processing.h"

void unlink_and_exit(char *name)
{
	(void)shm_unlink(name);
	exit(EXIT_FAILURE);
}

void *get_shared_memory_pointer(char *name, unsigned num_tries)
{
	unsigned tries;
	shmem_t *ptr;
	int fd;

	for (tries =0;;)
	{
		fd = shm_open(name, O_RDWR, 0);
		if (fd != -1) break;
		++tries;
		if (tries > num_tries)
		{
			perror("shmn_open");
			return MAP_FAILED;
		}
		/* wait one second then try again */
		sleep(1);
	}

	for (tries = 0;;)
	{
		ptr = mmap(0, sizeof(shmem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
		if (ptr != MAP_FAILED) break;
		++tries;
		if (tries > num_tries)
		{
			perror("mmap");
			return MAP_FAILED;
		}
		/* wait one second then try again */
		sleep(1);
	}

	/* no longer need fd */
	(void)close(fd);

	for(tries=0;;)
	{
		if (ptr->init_flag) break;
		++tries;
		if (tries > num_tries)
		{
			fprintf(stderr, "init flag never set \n");
			(void)munmap(ptr, sizeof(shmem_t));
			return MAP_FAILED;
		}
		/* wait one second then try again */
		sleep(1);
	}

	return ptr;
}

int receive_shm_processing(char * write_path)
{
	int fd;
	shmem_t *ptr;
	int ret;
	pthread_mutexattr_t mutex_attr;
	pthread_condattr_t cond_attr;
	uint64_t last_version = 0;

	FILE * fp;
	int total_read_bytes = 0;
	int length_msg = 0;
	char local_data_copy[MAX_TEXT_LEN];

	// create file
	fp = fopen(write_path, "w+");
	if (fp == NULL)
	{
		perror("fopen()");
		exit(-1);
	}

	printf("Creating shared memory object: %s \n", "/shmem");

	fd = shm_open("/shmem", O_RDWR | O_CREAT | O_EXCL, 0660);
	if (fd == -1)
	{
		perror("shm_open()");
		fclose(fp);
		unlink_and_exit("/shmem");
	}

	/* set the size of the shared memory object, allocating at least one page of memory */

	ret = ftruncate(fd, sizeof(shmem_t));
	if (ret == -1)
	{
		perror("ftruncate");
		fclose(fp);
		unlink_and_exit("/shmem");
	}

	/* get a pointer to the shared memory */

	ptr = mmap(0, sizeof(shmem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED)
	{
		perror("mmap");
		unlink_and_exit("/shmem");
	}

	/* don't need fd anymore, so close it */
	close(fd);

	/* now set up our mutex and condvar for the synchronization and notification */

	pthread_mutexattr_init(&mutex_attr);
	pthread_mutexattr_setpshared(&mutex_attr, PTHREAD_PROCESS_SHARED);
	ret = pthread_mutex_init(&ptr->mutex, &mutex_attr);
	if (ret != EOK)
	{
		perror("pthread_mutex_init");
		fclose(fp);
		unlink_and_exit("/shmem");
	}

	pthread_condattr_init(&cond_attr);
	pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
	ret = pthread_cond_init(&ptr->cond, &cond_attr);
	if (ret != EOK)
	{
		perror("pthread_cond_init");
		fclose(fp);
		unlink_and_exit("/shmem");
	}

	/*
	 * our memory is now "setup", so set the init_flag
	 * it was guaranteed to be zero at allocation time
	 */
	ptr->init_flag = 1;

	printf("Shared memory created and init_flag set to let users know shared memory object is usable \n");

	// read data, first data is the length of the text file

	while (length_msg == 0)
	{
		printf("waiting for length_msg \n");
		// 1. lock the mutex we're about to access shared data
		ret = pthread_mutex_lock(&ptr->mutex);
		if (ret != EOK)
		{
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

		// 2. wait for changes to the shared memory object
		while (last_version == ptr->data_version)
		{
			ret = pthread_cond_wait(&ptr->cond, &ptr->mutex); // does an unlock, wait, lock
			if (ret != EOK)
			{
				perror("pthread_cond_wait");
				exit(EXIT_FAILURE);
			}
		}

		// 3. copy length_msg
		last_version = ptr->data_version;
		strlcpy(local_data_copy, ptr->text, sizeof(local_data_copy));
		length_msg = atoi(local_data_copy);
		ptr->data_version = last_version+1;

		// 4. finished accessing shared data, unlock the mutex
		ret = pthread_mutex_unlock(&ptr->mutex);
		if (ret != EOK)
		{
			perror("pthread_mutex_unlock");
			fclose(fp);
			exit(EXIT_FAILURE);
		}
		//sleep(1);
	}

	printf("FILE SIZE: '%d' \n", length_msg);

	while (1)
	{
		// 1. lock the mutex we're about to access shared data
		ret = pthread_mutex_lock(&ptr->mutex);
		if (ret != EOK)
		{
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

		// 2. wait for changes to the shared memory object
		while (last_version == ptr->data_version)
		{
			ret = pthread_cond_wait(&ptr->cond, &ptr->mutex); // does an unlock, wait, lock
			if (ret != EOK)
			{
				perror("pthread_cond_wait");
				exit(EXIT_FAILURE);
			}
		}

		// 3. update data
		strlcpy(local_data_copy, ptr->text, sizeof(local_data_copy));
		printf("Received string: \"%s\" --> length is %d \n", local_data_copy, (int)strlen(local_data_copy));
		printf("read_bytes: %d \n", strlen(local_data_copy));
		last_version = ptr->data_version;
		total_read_bytes += strlen(local_data_copy);

		// write to file
		fputs(local_data_copy, fp);

		// 4. finished accessing shared data, unlock the mutex
		ret = pthread_mutex_unlock(&ptr->mutex);
		if (ret != EOK)
		{
			perror("pthread_mutex_unlock");
			fclose(fp);
			exit(EXIT_FAILURE);
		}

		// if msg received fully, we can stop
		if (total_read_bytes >= length_msg)
		{
			// close file
			fclose(fp);
			break;
		}

	}

	/* unmap() not actually needed on termination as all memory mappings are freed on process termination */
	if (munmap(ptr, sizeof(shmem_t)) == -1)
	{
		perror("munmap");
	}

	/* but the name must be removed */
	if (shm_unlink("/shmem") == -1)
	{
		perror("shm_unlink");
	}

	return EXIT_SUCCESS;


}

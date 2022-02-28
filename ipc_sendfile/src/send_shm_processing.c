/*
 * send_shm_processing.c
 *
 *  Created on: Feb 25, 2022
 *      Author: maksim
 */

#include "send_shm_processing.h"

#include <time.h>

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

int send_shm_processing(char * read_path)
{
	int ret;
	shmem_t *ptr;
	//uint64_t last_version = 0;
	char local_data_copy[MAX_TEXT_LEN];
	// client
	int fd;
	char readbuf[MAX_TEXT_LEN];
	// read file
	FILE *fp;
	int sz;	//size of the file
	size_t read_bytes;
	int n=0;
	int remaining_bytes;
	int exit_loop = 0;
	int last_version;

	// open file
	fp = fopen(read_path, "r");
	printf("READING FILE: %s \n", read_path);
	if (fp == NULL)
	{
		perror("Error opening file");
		return -1;
	}

	// find file size
	fseek(fp, 0L, SEEK_END);	// go to end
	sz = ftell(fp);				// store size
	rewind(fp);					// go to begin

	printf("FILE SIZE: %d \n", sz);

	/* try to get access to the shared memory object, retrying for 100 times */
	ptr = get_shared_memory_pointer("/shmem", 100);
	if (ptr == MAP_FAILED)
	{
		fprintf(stderr, "Unable to access object");
		exit(EXIT_FAILURE);
	}

	printf("shared memory pointer received \n");

	// send file size
	sprintf(readbuf, "%d", sz);

	/* 1. lock the mutex we're about to access shared data */
	ret = pthread_mutex_lock(&ptr->mutex);
	if (ret != EOK)
	{
		perror("pthread_mutex_lock");
		exit(EXIT_FAILURE);
	}

	ptr->data_version++;
	last_version = ptr->data_version;
	snprintf(ptr->text, sizeof(ptr->text), "%d", sz);

	/* 2. finished accessing shared data, unlock the mutex */
	ret = pthread_mutex_unlock(&ptr->mutex);
	if (ret != EOK)
	{
		perror("pthread_mutex_unlock");
		unlink_and_exit("/shmem");
	}

	/* 3. wake up any readers that may be waiting */
	ret = pthread_cond_broadcast(&ptr->cond);
	if (ret != EOK)
	{
		perror("pthread_cond_broadcast");
		unlink_and_exit("/shmem");
	}

	while (last_version == ptr->data_version)
	{
		delay(100);
	}


	while(1)
	{
		delay(100);

		/* lock the mutex we're about to access shared data */
		ret = pthread_mutex_lock(&ptr->mutex);
		if (ret != EOK)
		{
			perror("pthread_mutex_lock");
			exit(EXIT_FAILURE);
		}

		ptr->data_version++;
		read_bytes = fread(ptr->text, sizeof(ptr->text)-1, 1, fp);
		ptr->text[sizeof(readbuf)-1] = '\0';
		printf("read string: %s \n", ptr->text);

		if (read_bytes != 1)
		{
			//EOF reached, send last string
			remaining_bytes = sz - (sizeof(readbuf)-1)*n;
			ptr->text[remaining_bytes] = '\0';
			printf("last string: %s \n", ptr->text);
			printf("string length: %d \n", remaining_bytes);
			exit_loop = 1;
		}

		/* finished accessing shared data, unlock the mutex */
		ret = pthread_mutex_unlock(&ptr->mutex);
		if (ret != EOK)
		{
			perror("pthread_mutex_unlock");
			unlink_and_exit("/shmem");
		}

		/* wake up any readers that may be waiting */
		ret = pthread_cond_broadcast(&ptr->cond);
		if (ret != EOK)
		{
			perror("pthread_cond_broadcast");
			unlink_and_exit("/shmem");
		}

		if (exit_loop == 1)
		{
			break;
		}

		n+=1;
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

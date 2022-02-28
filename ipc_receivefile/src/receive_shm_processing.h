/*
 * receive_shm_processing.h
 *
 *  Created on: Feb 25, 2022
 *      Author: maksim
 */

#ifndef SRC_RECEIVE_SHM_PROCESSING_H_
#define SRC_RECEIVE_SHM_PROCESSING_H_

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <pthread.h>
#include <stdint.h>

#define MAX_TEXT_LEN 100

typedef struct
{
	volatile unsigned init_flag;	// has the shared memory and control structures been initialized
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	uint64_t data_version;	// for tracking updates, 64-bit count won't wrap during lifetime of a system
	char text[MAX_TEXT_LEN];
} shmem_t;

void unlink_and_exit(char *);

int receive_shm_processing(char *);

#endif /* SRC_RECEIVE_SHM_PROCESSING_H_ */

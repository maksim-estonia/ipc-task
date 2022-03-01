/*
 * send_queue_processing.h
 *
 *  Created on: Feb 23, 2022
 *      Author: maksim
 */

#ifndef SRC_SEND_QUEUE_PROCESSING_H_
#define SRC_SEND_QUEUE_PROCESSING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <time.h>

#define SERVER_QUEUE_NAME "/sp-example-server"
#define QUEUE_PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE (MAX_MSG_SIZE)

int send_queue_processing(char *);



#endif /* SRC_SEND_QUEUE_PROCESSING_H_ */

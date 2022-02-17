/*
 * pipe_processing.h
 *
 *  Created on: Feb 17, 2022
 *      Author: maksim
 */

#ifndef SRC_RECEIVE_PIPE_PROCESSING_H_
#define SRC_RECEIVE_PIPE_PROCESSING_H_

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "MYFIFO"

int receive_pipe_processing(char*);



#endif /* SRC_RECEIVE_PIPE_PROCESSING_H_ */

/*
 * pipe_processing.h
 *
 *  Created on: Feb 17, 2022
 *      Author: maksim
 */

#ifndef SRC_PIPE_PROCESSING_H_
#define SRC_PIPE_PROCESSING_H_

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define FIFO_FILE "MYFIFO"

int pipe_processing(void);



#endif /* SRC_PIPE_PROCESSING_H_ */

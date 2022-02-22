/*
 * receive_message_processing.h
 *
 *  Created on: Feb 22, 2022
 *      Author: maksim
 */

#ifndef SRC_RECEIVE_MESSAGE_PROCESSING_H_
#define SRC_RECEIVE_MESSAGE_PROCESSING_H_

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <process.h>
#include <string.h>

#include "msg_def.h"

#include <sys/iofunc.h>
#include <sys/dispatch.h>

typedef union
{
	uint16_t 	type;
	str_msg_t 		msg;
	struct _pulse pulse;
} recv_buf_t;

int receive_message_processing(char *);



#endif /* SRC_RECEIVE_MESSAGE_PROCESSING_H_ */

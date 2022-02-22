/*
 * send_message_processing.h
 *
 *  Created on: Feb 22, 2022
 *      Author: maksim
 */

#ifndef SRC_SEND_MESSAGE_PROCESSING_H_
#define SRC_SEND_MESSAGE_PROCESSING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/neutrino.h>
#include "msg_def.h"

#include <sys/netmgr.h>
#include <sys/iofunc.h>
#include <sys/dispatch.h>

int send_message_processing(char *);

#endif /* SRC_SEND_MESSAGE_PROCESSING_H_ */

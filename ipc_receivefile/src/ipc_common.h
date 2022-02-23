/*
 * ipc_common.h
 *
 *  Created on: Feb 21, 2022
 *      Author: maksim
 */

#ifndef SRC_IPC_COMMON_H_
#define SRC_IPC_COMMON_H_

#include <getopt.h>

/* command line options (for getopt_long) */
static struct option long_options[] =
{
	{"help", 		no_argument,		0, 'h'},
	{"messages",	no_argument,		0, 'm'},
	{"queue",		no_argument, 		0, 'q'},
	{"pipe",		no_argument,		0, 'p'},
	{"shm",			no_argument,		0, 's'},
	{"file",		required_argument,	0, 'f'},
	{0, 0, 0, 0}
};

typedef enum
{
	DEFAULT,
	MESSAGE,
	QUEUE,
	PIPE,
	SHM
} Transport_type;

// structure to store the arguments for a given command
typedef struct
{
	Transport_type 	tr_type;
	char 			*read_path;	//string that stores path to file we send/read from
	char			*write_path;	//string that stores path we write to
} Arguments;

#endif /* SRC_IPC_COMMON_H_ */

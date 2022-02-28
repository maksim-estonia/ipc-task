#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msg_def.h"
#include <errno.h>
#include <sys/dispatch.h>

#include <getopt.h>

#include "ipc_common.h"
#include "receive_pipe_processing.h"

#include "receive_message_processing.h"


#include "receive_queue_processing.h"

//typedef union
//{
//	uint16_t type;
//	cksum_msg_t msg;
//	struct _pulse pulse;
//} recv_buf_t;

int main(int argc, char* argv[])
{
	int c;
	Arguments arg;
	int status = 0;

	/* Default arguments values */
	arg.tr_type = DEFAULT;
	arg.read_path = NULL;
	arg.write_path = NULL;

	/* if not enough arguments given */
	if (argc < 2)
	{
		fprintf(stderr, "ipc_receivefile requires arguments \n");
		return -1;
	}

	while(1)
	{
		int option_index = 0;

		c = getopt_long(argc, argv, "hmqpsf:", long_options, &option_index);

		/* detect the end of options */
		if (c == -1)
			break;

		switch (c)
		{
		case 'm':
			arg.tr_type 	= MESSAGE;
			//(&arg)->read_path 	= optarg;
			break;
		case 'q':
			arg.tr_type 	= QUEUE;
			//(&arg)->read_path	= optarg;
			break;
		case 'p':
			arg.tr_type		= PIPE;
			//(&arg)->read_path	= optarg;
			break;
		case 's':
			arg.tr_type		= SHM;
			//(&arg)->read_path	= optarg;
			break;
		case 'f':
			arg.write_path	= optarg;
			break;

		case 'h':
			printf("\n Use the program as follows: \n");
			printf("--help: prints out a help text containing short description of all supported command-line arguments \n");
			printf("--message <TBD>: use message passing as IPC method \n");
			printf("--queue <TBD>: use message queue as IPC method \n");
			printf("--pipe <TBD>: use pipe as IPC method \n");
			printf("--shm [buffer size in kB]: use shared memory buffer as IPC method \n");
			printf("--file [filename]: file used to read data from \n");
			return -1;

		default:
			fprintf(stderr, "Unknown arguments provided \n");
			return -1;
		}
	}

	if (!arg.write_path)
	{
		fprintf(stderr, "No file path provided \n");
		return -1;
	}

	switch (arg.tr_type)
	{

	case SHM:
		fprintf(stderr, "This type of ipc is not yet implemented \n");
		return -1;
	case QUEUE:
		printf("QUEUE selected \n");
		status = receive_queue_processing(arg.write_path);
		if (status != 0)
		{
			fprintf(stderr, "QUEUE failed \n");
			return -1;
		}
		printf("QUEUE received");
		break;
	case MESSAGE:
		printf("MESSAGE selected \n");
		status = receive_message_processing(arg.write_path);
		if (status != 0)
		{
			fprintf(stderr, "MESSAGE failed \n");
			return -1;
		}
		printf("MESSAGE received \n");
		break;
	case PIPE:
		printf("PIPE messaging selected \n");
		status = 0;
		status = receive_pipe_processing(arg.write_path);
		if (status != 0)
		{
			fprintf(stderr, "PIPE receive failed \n");
			return -1;
		}
		printf("PIPE message received \n");
		break;
	default:
		fprintf(stderr, "No IPC message type selected \n");
		return -1;
	}

	return 0;





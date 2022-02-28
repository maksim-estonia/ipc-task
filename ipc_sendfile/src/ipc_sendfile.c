#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "ipc_common.h"
#include "send_pipe_processing.h"

#include "send_message_processing.h"

#include "send_queue_processing.h"

#include "send_shm_processing.h"

int main(int argc, char* argv[])
{

	int c;
	Arguments arg;
	int status = 0;

	/* Default arguments values */
	(&arg)->tr_type	= DEFAULT;
	(&arg)->read_path	= NULL;
	(&arg)->write_path = NULL;

	/* if not enough arguments given */
	if (argc < 2)
	{
		fprintf(stderr, "ipc_sendfile requires arguments! \n");
		return -1;
	}


	while(1)
	{
		/* getopt_long stores the option index here */
		int option_index = 0;

		c = getopt_long(argc, argv, "hmqpsf:", long_options, &option_index);

		/* detect the end of options */
		if (c == -1)
		{
			break;
		}

		switch (c)
		{
		case 'm':
			(&arg)->tr_type 	= MESSAGE;
			//(&arg)->read_path 	= optarg;
			break;
		case 'q':
			(&arg)->tr_type 	= QUEUE;
			//(&arg)->read_path	= optarg;
			break;
		case 'p':
			(&arg)->tr_type		= PIPE;
			//(&arg)->read_path	= optarg;
			break;
		case 's':
			(&arg)->tr_type		= SHM;
			//(&arg)->read_path	= optarg;
			break;
		case 'f':
			(&arg)->read_path	= optarg;
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

	if (!(&arg)->read_path)
	{
		fprintf(stderr, "No file path provided \n");
		return -1;
	}

	switch ((&arg)->tr_type)
	{
	case SHM:
		printf("SHARED MEMORY selected \n");
		status = 0;
		status = send_shm_processing((&arg)->read_path);
		if (status != 0)
		{
			fprintf(stderr, "SHARED MEMORY failed \n");
			return -1;
		}
		printf("SHARED MEMORY sent \n");
		break;
	case QUEUE:
		printf("QUEUE selected \n");
		status = 0;
		status = send_queue_processing((&arg)->read_path);
		if (status != 0)
		{
			fprintf(stderr, "QUEUE failed \n");
			return -1;
		}
		printf("QUEUE sent \n");
		break;
	case MESSAGE:
		printf("MESSAGE selected \n");
		status = 0;
		status = send_message_processing((&arg)->read_path);
		if (status != 0)
		{
			fprintf(stderr, "MESSAGE failed \n");
			return -1;
		}
		printf("MESSAGE sent \n");
		break;
	case PIPE:
		printf("PIPE messaging selected \n");
		status = 0;
		status = send_pipe_processing((&arg)->read_path);
		if (status != 0)
		{
			fprintf(stderr, "PIPE messaging failed \n");
			return -1;
		}
		printf("PIPE message sent \n");
		break;
	default:
		fprintf(stderr, "No IPC message type selected \n");
		return -1;
	}

	return 0;

}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "msg_def.h"
#include <errno.h>
#include <sys/dispatch.h>

// pipe processing
#include "pipe_processing.h"

//int calculate_checksum(char *text);

//void message_processing(void);

typedef union
{
	uint16_t type;
	cksum_msg_t msg;
	struct _pulse pulse;
} recv_buf_t;

int main(int argc, char* argv[])
{
	int counter;

	if (argc == 1)
	{
		printf("ERROR: Program has to be run with command-line arguments, for more information: \n\n");
		printf("ipc_receivefile --help");
		return EXIT_FAILURE;
	}

	if (argc >= 2)
	{
		printf("\n Arguments passed: \n");
		for (counter=0; counter<argc; counter++)
			printf("\n argv[%d]: %s", counter, argv[counter]);
		printf("\n \n");
	}

	//--help
	if (strcmp(argv[1], "--help") == 0)
	{
		printf("\n Use the program as follows: \n");
		printf("--help: prints out a help text containing short description of all supported command-line arguments \n");
		printf("--message <TBD>: use message passing as IPC method \n");
		printf("--queue <TBD>: use message queue as IPC method \n");
		printf("--pipe <TBD>: use pipe as IPC method \n");
		printf("--shm [buffer size in kB]: use shared memory buffer as IPC method \n");
		printf("--file [filename]: file used to write data to \n");
	}

	// if only --help, otherwise we need at least 2 arguments (ipc method and file)
	if (argc == 2)
	{
		return EXIT_SUCCESS;
	}

	//--message
	if (strcmp(argv[1], "--message") == 0)
	{
		printf("Using message passing as IPC method \n");

		if (strcmp(argv[2], "--file") == 0)
		{
			printf("Writing data to: %s", argv[3]);
		}

		//message_processing();
	}

	//--queue
	if (strcmp(argv[1], "--queue") == 0)
	{
		printf("Using message queue as IPC method \n");

		if (strcmp(argv[2], "--file") == 0)
		{
			printf("Writing data to: %s", argv[3]);
		}
	}

	//--pipe
	if (strcmp(argv[1], "--pipe") == 0)
	{
		printf("Using pipe as IPC method \n");

		if (strcmp(argv[2], "--file") == 0)
		{
			printf("Writing data to: %s", argv[3]);
		}

		pipe_processing();
	}

	//--shm
	if (strcmp(argv[1], "--shm") == 0)
	{
		printf("Using shared memory as IPC method \n");

		if (strcmp(argv[2], "--file") == 0)
		{
			printf("Writing data to: %s", argv[3]);
		}
	}


	return EXIT_SUCCESS;
}

//int calculate_checksum(char *text)
//{
//	char *c;
//	int cksum = 0;
//
//	for (c = text; *c != '\0'; c++)
//		cksum += *c;
//
//	return cksum;
//}

//void message_processing(void)
//{
//	name_attach_t *att;
//	int rcvid;
//	recv_buf_t rbuf;
//	int status;
//	int checksum;
//
//	// register our name
//	att = name_attach(NULL, SERVER_NAME, 0);
//
//	if (att == NULL)
//	{
//		perror("name_attach");
//		exit(EXIT_FAILURE);
//	}
//
//	while (1)
//	{
//		// receive msg from client
//		rcvid = MsgReceive(att->chid, &rbuf, sizeof(rbuf), NULL);
//		if (rcvid == -1)
//		{
//			perror("MsgReceive");
//			exit(EXIT_FAILURE);
//		}
//		else
//		{
//			if (rbuf.type == CKSUM_MSG_TYPE)
//			{
//				printf("Got a message \n");
//				checksum = calculate_checksum(rbuf.msg.string_to_cksum);
//				status = MsgReply(rcvid, EOK, &checksum, sizeof(checksum));
//				if (status == -1)
//				{
//					perror("MsgReply");
//					exit(EXIT_FAILURE);
//				}
//			}
//		}
//	}
//}



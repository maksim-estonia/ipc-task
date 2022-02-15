#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{

	int counter;

	if (argc == 1)
	{
		printf("ERROR: Program has to be run with command-line arguments, for more information: \n\n");
		printf("ipc_sendfile --help");
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
		printf("--file [filename]: file used to read data from \n");

	}

	// if only --help, otherwise we need at least 2 arguments (ipc method and file)
	if (argc == 2)
	{
		return EXIT_SUCCESS;
	}

	//--message
	if (strcmp(argv[1], "--message") == 0)
	{
		printf("\n Using message passing as IPC method \n");

		if (strcmp(argv[2], "--file") == 0)
		{
			printf("Sending data from: %s", argv[3]);
		}
	}

	//--queue
	if (strcmp(argv[1], "--queue") == 0)
	{
		printf("\n Using message queue as IPC method \n");

		if (strcmp(argv[2], "--file") == 0)
		{
			printf("Sending data from: %s", argv[3]);
		}
	}

	//--pipe
	if (strcmp(argv[1], "--pipe") == 0)
	{
		printf("\n Using pipe as IPC method \n");

		if (strcmp(argv[2], "--file") == 0)
		{
			printf("Sending data from: %s", argv[3]);
		}
	}

	//--shm
	if (strcmp(argv[1], "--shm") == 0)
	{
		printf("\n Using shared memory as IPC method \n");

		if (strcmp(argv[2], "--file") == 0)
		{
			printf("Sending data from: %s", argv[3]);
		}
	}



	return EXIT_SUCCESS;
}

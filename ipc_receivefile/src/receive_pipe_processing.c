/*
 * pipe_processing.c: receiving a text file over a pipe
 *
 *  Created on: Feb 17, 2022
 *      Author: maksim
 */

#include "receive_pipe_processing.h"

// returns 0 if succesful
int receive_pipe_processing(char* filename)
{
	int fd;
	char readbuf[80];
	char end[5];
	int to_end;
	int read_bytes;

	FILE * fp;

	// create file
	fp = fopen(filename, "w+");

	// create a FIFO
	mknod(FIFO_FILE, S_IFIFO|0640, 0);
	strcpy(end, "end");

	while(1)
	{
		// open pipe and read data
		fd = open(FIFO_FILE, O_RDONLY);
		read_bytes = read(fd, readbuf, sizeof(readbuf));
		readbuf[read_bytes] = '\0';
		printf("Sent string: %s --> string length is %d\n", readbuf, (int)strlen(readbuf));

		// if end we can stop
		to_end = strcmp(readbuf, end);
		if (to_end == 0)
		{
			// close pipe
			close(fd);
			// close file
			fclose(fp);
			break;
		}

		// if not end, write to file
		fputs(readbuf, fp);
	}

	return 0;


}

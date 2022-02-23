/*
 * pipe_processing.c: receiving a text file over a pipe
 *
 *  Created on: Feb 17, 2022
 *      Author: maksim
 */

#include "receive_pipe_processing.h"

#define BUFFER_SIZE 1024

// returns 0 if succesful
int receive_pipe_processing(char* write_path)
{
	int fd;
	char readbuf[BUFFER_SIZE];
	int read_bytes;
	int total_read_bytes = 0;
	int length_msg = 0;

	FILE * fp;

	// create file
	fp = fopen(write_path, "w+");

	// create a FIFO
	mknod(FIFO_FILE, S_IFIFO|0640, 0);

	// open pipe, read data, first data is the length of the text file
	fd = open(FIFO_FILE, O_RDONLY);
	while (length_msg == 0)
	{
		read_bytes = read(fd, readbuf, sizeof(readbuf));
		length_msg = atoi(readbuf);
	}
	printf("FILE SIZE: %d \n", length_msg);

	while(1)
	{
		// open pipe and read data, first data is the length of the text file
		//fd = open(FIFO_FILE, O_RDONLY);
		read_bytes = read(fd, readbuf, sizeof(readbuf)-1);
		printf("read_bytes: %d \n", read_bytes);
		total_read_bytes += read_bytes;
		readbuf[read_bytes] = '\0';
		printf("Received string: \"%s\" --> length is %d \n", readbuf, (int)strlen(readbuf));

		//write to file
		fputs(readbuf, fp);

		//if msg received fully, we can stop
		if (total_read_bytes >= length_msg)
		{
			// close pipe
			close(fd);
			// close file
			fclose(fp);
			break;
		}
	}

	return 0;


}

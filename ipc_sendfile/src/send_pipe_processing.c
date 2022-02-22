/*
 * pipe_processing.c: sending a text file over a pipe
 *
 *  Created on: Feb 17, 2022
 *      Author: maksim
 */

#include "send_pipe_processing.h"

#define BUFFER_SIZE 1024

int send_pipe_processing(char* read_path)
{
	// client
	int fd;
	char readbuf[BUFFER_SIZE];
	// read file
	FILE *fp;
	int sz; //size of the file
	size_t read_bytes;
	int n = 0;
	int remaining_bytes;

	// open file
	fp = fopen(read_path, "r");
	printf("READING FILE: %s \n", read_path);
	if (fp == NULL)
	{
		perror("Error opening file");
		return -1;
	}

	// find file size
	fseek(fp, 0L, SEEK_END);	// go to end
	sz = ftell(fp);				// store size
	rewind(fp);					// go to begin

	printf("FILE SIZE: %d \n", sz);

	// open pipe
	printf("Start sending messages \n");
	fd = open(FIFO_FILE, O_CREAT|O_WRONLY);

	// send file size
	sprintf(readbuf, "%d", sz);
	write(fd, readbuf, strlen(readbuf));
	delay(100);

	while(1)
	{
		read_bytes = fread(readbuf, sizeof(readbuf)-1, 1, fp);
		printf("size: %d \n", sizeof(readbuf)-1);

		if (read_bytes != 1)
		{
			//EOF reached, send last string
			remaining_bytes = sz - (sizeof(readbuf)-1)*n;
			readbuf[remaining_bytes] = '\0';
			printf("last string: %s \n", readbuf);
			printf("string length: %d \n", remaining_bytes);
			write(fd, readbuf, remaining_bytes);
			delay(100);

			// file has been sent
			// close file
			fclose(fp);
			// close pipe
			close(fd);

			break;
		}

		//sending full buffer
		readbuf[sizeof(readbuf)-1] = '\0';
		printf("read string: %s \n", readbuf);
		write(fd, readbuf, sizeof(readbuf));
		delay(100);
		n +=1;
	}

	return 0;

}

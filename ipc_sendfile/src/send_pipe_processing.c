/*
 * pipe_processing.c: sending a text file over a pipe
 *
 *  Created on: Feb 17, 2022
 *      Author: maksim
 */

#include "send_pipe_processing.h"

// returns 0 if successful, -1 if not
int send_pipe_processing(char* filename)
{
	// client
	int fd;
	int stringlen;
	char end_str[5];
	// read file
	FILE *fp;
	char str[80];

	// open file
	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		perror("Error opening file");
		return -1;
	}

	// open pipe
	printf("FIFO_CLIENT: send messages \n");
	fd = open(FIFO_FILE, O_CREAT|O_WRONLY);
	strcpy(end_str, "end");

	while (fgets(str, 80, fp) != NULL)
	{
		// send str through pipe
		stringlen = strlen(str);
		str[stringlen] = '\0';

		write(fd, str, strlen(str));
		printf("Sent string: %s --> string length is %d\n", str, (int)strlen(str));

		delay(100);
	}

	// close file
	fclose(fp);

	// close pipe
	close(fd);

	return 0;


}

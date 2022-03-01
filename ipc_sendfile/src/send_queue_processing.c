/*
 * send_queue_processing.c
 *
 *  Created on: Feb 23, 2022
 *      Author: maksim
 */

#include "send_queue_processing.h"

int send_queue_processing(char * read_path)
{
	mqd_t qd_server;
	int status;
	char readbuf[MSG_BUFFER_SIZE];
	//read file
	FILE * fp;
	int sz; // size of the file
	size_t read_bytes;
	int n = 0;
	int remaining_bytes;

	// open file
	fp = fopen(read_path, "r");
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

	if ( (qd_server = mq_open(SERVER_QUEUE_NAME, O_WRONLY)) == -1 )
	{
		perror("Client: mq_open (server)");
		fclose(fp);
		exit(-1);
	}

	// send file size
	sprintf(readbuf, "%d", sz);
	status = mq_send(qd_server, readbuf, strlen(readbuf), 0);
	if (status == -1)
	{
		perror("Client: not able to send message to server \n");
		fclose(fp);
		exit(1);
	}
	delay(100);

	while(1)
	{
		read_bytes = fread(readbuf, sizeof(readbuf)-1, 1, fp);

		if (read_bytes != 1)
		{
			// EOF reached, send last string
			remaining_bytes = sz - (sizeof(readbuf)-1)*n;
			readbuf[remaining_bytes] = '\0';
			printf("last string length: %d \n", remaining_bytes);
			status = mq_send(qd_server, readbuf, remaining_bytes, 0);
			if (status == -1)
			{
				perror("Client: not able to send message to server \n");
				fclose(fp);
				exit(-1);
			}
			delay(100);
			fclose(fp);
			break;
		}

		// sending full buffer
		readbuf[sizeof(readbuf)-1] = '\0';
		printf("string length: %d \n", (int)strlen(readbuf));
		status = mq_send(qd_server, readbuf, sizeof(readbuf), 0);
		if (status == -1)
		{
			perror("Client: not able to send message to server \n");
			fclose(fp);
			exit(-1);
		}
		delay(100);
		n +=1;
	}

	return 0;
}

/*
 * receive_queue_processing.c
 *
 *  Created on: Feb 23, 2022
 *      Author: maksim
 */

#include "receive_queue_processing.h"

int receive_queue_processing(char * write_path)
{
	mqd_t qd_server;	//queue descriptors
	int read_bytes;
	int total_read_bytes = 0;
	int length_msg = 0;

	char rbuf[MSG_BUFFER_SIZE];

	struct mq_attr attr;
	attr.mq_flags = 0;
	attr.mq_maxmsg = MAX_MESSAGES;
	attr.mq_msgsize = MAX_MSG_SIZE;
	attr.mq_curmsgs = 0;

	printf("Receive side started \n");

	FILE * fp;

	// create file
	fp = fopen(write_path, "w+");
	if (fp == NULL)
	{
		perror("fopen");
		exit(-1);
	}

	// open queue
	qd_server = mq_open(SERVER_QUEUE_NAME, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr);
	if (qd_server == -1)
	{
		perror("Server: mq_open");
		fclose(fp);
		exit(-1);
	}

	//first data is the length of the text file
	while (length_msg == 0)
	{
		read_bytes = mq_receive(qd_server, rbuf, MSG_BUFFER_SIZE, NULL);
		if (read_bytes == -1)
		{
			perror("Server: mq_receive");
			fclose(fp);
			exit(-1);
		}

		length_msg = atoi(rbuf);
	}

	printf("FILE SIZE: %d \n", length_msg);

	while(1)
	{
		// read data
		read_bytes = mq_receive(qd_server, rbuf, MSG_BUFFER_SIZE-1, NULL);
		rbuf[read_bytes] = '\0';
		total_read_bytes += (int)strlen(rbuf);
		printf("Received string: \"%s\" --> length is %d \n", rbuf, (int)strlen(rbuf));

		//write to file
		fputs(rbuf, fp);

		// if msg received fully, we can stop
		if (total_read_bytes >= length_msg)
		{
			// close queue
			mq_close(qd_server);
			// close file
			fclose(fp);
			break;
		}
	}

	return 0;
}

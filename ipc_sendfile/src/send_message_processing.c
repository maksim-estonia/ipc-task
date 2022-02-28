/*
 * send_message_processing.c
 *
 *  Created on: Feb 22, 2022
 *      Author: maksim
 */

#include "send_message_processing.h"

int send_message_processing(char * read_path)
{
	int coid;
	str_msg_t msg;
	int status;
	size_t read_elements;

	int sz;
	int remaining_bytes;
	int n = 0;

	//read file
	FILE *fp;

	//open file
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

	printf("FILE SIZE : %d \n", sz);

	coid = name_open(SERVER_NAME, 0);
	if (coid == -1)
	{
		perror("ConnectAttach");
		fclose(fp);
		exit(EXIT_FAILURE);
	}
	msg.msg_type = STR_MSG_TYPE;

	while(1)
	{
		read_elements = fread(msg.string, sizeof(msg.string)-1, 1, fp);

		if (read_elements != 1)
		{
			//EOF reached, send last string
			remaining_bytes = sz - (sizeof(msg.string)-1)*n;
			msg.string[remaining_bytes] = '\0';
			printf("last_string: %s \n", msg.string);
			printf("string length: %d \n", remaining_bytes);

			// send
			status = MsgSend(coid, &msg, sizeof(msg.msg_type) + strlen(msg.string) + 1, NULL, 0);
			if (status == -1)
			{
				perror("MsgSend");
				fclose(fp);
				exit(EXIT_FAILURE);
			}

			break;
		}

		//sending full
		msg.string[sizeof(msg.string)-1] = '\0';
		printf("read string: %s \n", msg.string);
		status = MsgSend(coid, &msg, sizeof(msg.msg_type) + strlen(msg.string) + 1, NULL, 0);
		n +=1;

	}

//	coid = name_open(SERVER_NAME, 0);
//	if (coid == -1)
//	{
//		perror("ConnectAttach");
//		exit(EXIT_FAILURE);
//	}
//
//	msg.msg_type = STR_MSG_TYPE;
//	strncpy(msg.string, "Hello", MAX_STRING_LEN);
//	printf("Sending string: %s \n", msg.string);
//
//	status = MsgSend(coid, &msg, sizeof(msg.msg_type) + strlen(msg.string) + 1, NULL, 0);
//
//	if (status == -1)
//	{
//		perror("MsgSend");
//		exit(EXIT_FAILURE);
//	}
//
//	printf("MsgSend return status: %d \n", status);

	return 0;
}

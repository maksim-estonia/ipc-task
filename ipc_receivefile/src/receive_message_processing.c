/*
 * receive_message_processing.c
 *
 *  Created on: Feb 22, 2022
 *      Author: maksim
 */

#include "receive_message_processing.h"

int receive_message_processing(char * write_path)
{
	name_attach_t 	*att;
	int rcvid;
	recv_buf_t 		rbuf;
	int status;

	FILE * fp;

	// create file
	fp = fopen(write_path, "w+");
	if (fp == NULL)
	{
		perror("fopen");
		exit(EXIT_FAILURE);
	}

	// register our name
	att = name_attach(NULL, SERVER_NAME, 0);

	if (att == NULL)
	{
		perror("name_attach");
		fclose(fp);
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		// receive msg from client
		rcvid = MsgReceive(att->chid, &rbuf, sizeof(rbuf), NULL);
		if (rcvid == -1)
		{
			perror("MsgReceive");
			fclose(fp);
			exit(EXIT_FAILURE);
		}
		else if (rcvid == 0)
		{
			printf("we got a pulse with a code of %d, and value of %d\n", rbuf.pulse.code,
													rbuf.pulse.value.sival_int);
			if (rbuf.pulse.code == _PULSE_CODE_DISCONNECT)
			{
				printf("Client disconnected, copy finished \n");
				if (ConnectDetach(rbuf.pulse.scoid) == -1)
				{
					perror("ConnectDetach");
				}
				//close file
				fclose(fp);
				break;
			}
		}
		else
		{
			if (rbuf.type == STR_MSG_TYPE)
			{
				printf("Received string: \"%s\" --> length is %d \n", rbuf.msg.string, (int)strlen(rbuf.msg.string));
				// write to file
				fputs(rbuf.msg.string, fp);
				// confirm receive by replying
				status = MsgReply(rcvid, EOK, NULL, 0);
				if (status == -1)
				{
					perror("MsgReply");
					fclose(fp);
					exit(EXIT_FAILURE);
				}
			}
			else
			{
				perror("MsgError");
				fclose(fp);
				exit(EXIT_FAILURE);
			}
		}
	}
	return 0;
}

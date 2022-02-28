#ifndef _MSG_DEF_H_
#define _MSG_DEF_H_

#include <sys/iomsg.h>

#define MAX_STRING_LEN    256
#define STR_MSG_TYPE (_IO_MAX + 1)

//layout of msgs should always defined by a struct, and ID'd by a msg type
// number as the first member
typedef struct
{
	uint16_t msg_type;
	char string[MAX_STRING_LEN + 1];
} str_msg_t;

// checksum reply is an int

// If you are sharing a target with other people, please customize these server names
// so as not to conflict with the other person.

#define CKSUM_PULSE_CODE _PULSE_CODE_MINAVAIL+3

#define PULSE_PRIORITY -1

#define SERVER_NAME		"ipc_receive_server"

//#define DISCONNECT_SERVER "disconnect_server"

//#define UNBLOCK_SERVER "unblock_server"

#endif //_MSG_DEF_H_

#ifndef __PARAMETER_H
#define __PARAMETER_H

#include "mavlink.h"

#define PARAMETER_MSG_DEF(id, handler) \
        {.name = #id, .message_handler = handler, .msgid = id}

enum {
	PARAMETER_IDLE,
	/* Parameter read protocol */
	PARAMETER_READ_REQUEST,
	/* Parameter read single protocol */
	PARAMETER_READ_SINGLE_REQUEST,
	/* Parameter write protocol */
	PARAMETER_SET_REQUEST
} ParameterState;

struct mission_parser_data {
        uint8_t msgid;
        char *name;
        void (*message_handler)(mavlink_message_t *mavlink_message);
};

bool parameter_handle_message(mavlink_message_t *mavlink_message);
void parameter_send(void);

#endif

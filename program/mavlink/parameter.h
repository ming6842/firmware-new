#ifndef __PARAMETER_H
#define __PARAMETER_H

#include "mavlink.h"

#define PARAMETER_MSG_DEF(id, handler) \
        {.name = #id, .message_handler = handler, .msgid = id}

enum {
	PARAMETER_STATE_IDLE,
	/* Parameter read protocol */
	PARAMETER_READ_REQUEST,
	/* Parameter read single protocol */
	PARAMETER_READ_SINGLE_REQUEST,
	/* Parameter write protocol */
	PARAMETER_SET_REQUEST
} ParameterState;

typedef struct {
	/* Mavlink transaction state */
	int mavlink_state;
	uint32_t timeout_start_time;
	uint32_t last_retry_time;
} parameter_info_t;

struct mission_parser_data {
        uint8_t msgid;
        char *name;
        void (*message_handler)(mavlink_message_t *mavlink_message);
};

bool parameter_handle_message(mavlink_message_t *mavlink_message);
int get_mavlink_parameter_state(void);
void parameter_send(void);

#endif

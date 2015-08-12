#ifndef __COMMAND_PARSER_H
#define __COMMAND_PARSER_H

#include <stdbool.h>

#include "mavlink.h"

#define GENERIC_MSG_DEF(id, handler) \
	{.name = #id, .message_handler = handler, .msgid = id}

/* Remove this later! */
typedef struct {
	uint32_t timeout_start_time;
	int state;
} transaction_state_t;

struct generic_parser_data {
	uint8_t msgid;
	char *name;
	void (*message_handler)(mavlink_message_t *mavlink_message);
};

bool generic_handle_message(mavlink_message_t *mavlink_message);

#endif

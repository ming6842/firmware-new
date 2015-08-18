#ifndef __COMMAND_PARSER_H
#define __COMMAND_PARSER_H

#include <stdbool.h>

#include "mavlink.h"

#define GENERIC_MSG_DEF(id, handler) \
	{.name = #id, .message_handler = handler, .msgid = id}

struct generic_parser_item {
	uint8_t msgid;
	char *name;
	void (*message_handler)(mavlink_message_t *mavlink_message);
};

bool generic_handle_message(mavlink_message_t *mavlink_message);

#endif

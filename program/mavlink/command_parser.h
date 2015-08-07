#ifndef __COMMAND_PARSER_H
#define __COMMAND_PARSER_H

#include <stdbool.h>

#include "mavlink.h"

typedef struct {
	uint32_t timeout_start_time;
	int state;
} transaction_state_t;

struct mavlink_cmd {
	uint8_t msgid;
	char *name;
	void (*cmd_handler)(void);
};

bool generic_handle_message(mavlink_message_t *mavlink_message);
void mavlink_parse_received_cmd(mavlink_message_t *msg);

#endif

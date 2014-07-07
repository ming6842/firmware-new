#ifndef __COMMAND_PARSER_H
#define __COMMAND_PARSER_H

struct mavlink_cmd {
	uint8_t msgid;
	void (*cmd_handler)(void);
};

void mavlink_parse_received_cmd(mavlink_message_t *msg);

#endif

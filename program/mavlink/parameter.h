#ifndef __PARAMETER_H
#define __PARAMETER_H

#include "mavlink.h"

#define PARAMETER_MSG_DEF(id, handler) \
        {.name = #id, .message_handler = handler, .msgid = id}

typedef struct {
	bool active_to_send;
	int sent_count;
	int send_index;
	uint32_t last_send_time;
} parameter_info_t;

struct parameter_parser_item {
        uint8_t msgid;
        char *name;
        void (*message_handler)(mavlink_message_t *mavlink_message);
};

bool parameter_handle_message(mavlink_message_t *mavlink_message);
int get_mavlink_parameter_state(void);
void parameter_send(void);

#endif

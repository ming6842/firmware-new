#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <stddef.h>
#include <stdlib.h>
#include "mavlink.h"

struct mavlink_cmd {
	uint8_t msgid;
	void (*cmd_handler)();
};

void send_package(uint8_t *buf, mavlink_message_t *msg);

void ground_station_send_task();
void ground_station_receive_task();

#endif

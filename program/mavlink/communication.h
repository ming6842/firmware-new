#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "mavlink.h"
#include "communication.h"

struct mavlink_cmd {
	uint8_t msgid;
	void (*cmd_handler)(void);
};

void send_package(mavlink_message_t *msg);

void ground_station_send_task(void);
void ground_station_receive_task(void);

#endif

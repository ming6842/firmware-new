#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "mavlink.h"
#include "communication.h"

enum {
	BROADCAST_TIMER_1HZ,
	BROADCAST_TIMER_20HZ,
	BROADCAST_TIMER_CNT
} BroadcastTimerCount;

void mavlink_broadcast_task_timeout_check(void);

void receiver_task_send_package(mavlink_message_t *msg);

void send_status_text_message(char *text, uint8_t severity);

void mavlink_receiver_task(void);
void mavlink_broadcast_task(void);

#endif

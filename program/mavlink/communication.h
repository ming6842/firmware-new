#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "mavlink.h"
#include "communication.h"

#define BROADCAST_MESSAGE_CNT (sizeof(boradcast_message_list) / sizeof(boradcast_message_list[0]))

#define BROADCAST_MSG_DEF(send_function, tick_time) \
        {.send_message = send_function, .period_tick = tick_time}

#define RATE_HZ(hz) (1000 / hz)

/* Debug print configuration */
#define PRINT_MAVLINK_DEBUG_MESSAGE 1

#if PRINT_MAVLINK_DEBUG_MESSAGE != 0
	#define MAVLINK_DEBUG_PRINT printf
#else
	#define MAVLINK_DEBUG_PRINT(...) //Dummy
#endif

typedef struct {
	uint32_t period_tick;
	uint32_t last_send_time;
	void (*send_message)(void);
} broadcast_message_t;

void set_mavlink_receiver_delay_time(uint32_t time);

void receiver_task_send_package(mavlink_message_t *msg);
void send_status_text_message(char *text, uint8_t severity);

void mavlink_receiver_task(void);
void mavlink_broadcast_task(void);

#endif

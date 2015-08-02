#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "mavlink.h"
#include "communication.h"

enum {
	WAYPOINT_WRITE_PROTOCOL,
	WAYPOINT_READ_PROTOCOL,
	PARAMETER_WRITE_PROTOCOL,
	PARAMETER_READ_PROTOCOL,
	PARAMETER_READ_SINGLE_PROTOCOL
} TransactionType;

void send_package(mavlink_message_t *msg);
void clear_message_id(mavlink_message_t *message);

void transaction_begin(int type);
void transaction_end(void);
void reset_transaction_timer(void);

void ground_station_task(void);
void mavlink_send_task(void);

#endif

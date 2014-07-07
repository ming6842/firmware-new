#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "mavlink.h"
#include "communication.h"

void send_package(mavlink_message_t *msg);
void clear_message_id(mavlink_message_t *message);

void ground_station_task(void);
void mavlink_receiver_task(void);

#endif

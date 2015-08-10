#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "mavlink.h"
#include "communication.h"

void receiver_task_send_package(mavlink_message_t *msg);

void send_status_text_message(char *text);

void mavlink_receiver_task(void);
void mavlink_broadcast_task(void);

#endif

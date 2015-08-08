#include "mavlink.h"

#include "communication.h"
#include "command_parser.h"

#define REGISTERED_MISSION_MSG_CNT (sizeof(generic_message_list) / sizeof(generic_message_list[0]))

#define GENERIC_DEBUG_PRINT printf

static void heartbeat_handler(mavlink_message_t *mavlink_message);

struct generic_parser_data generic_message_list[] = {
	GENERIC_MSG_DEF(MAVLINK_MSG_ID_HEARTBEAT, heartbeat_handler)
};

/**
  * @brief  Try to parse and handle the passed message
  * @param  Received mavlink message (pointer) 
  * @retval The message can be handled or not (is this a generic message?)
  */
bool generic_handle_message(mavlink_message_t *mavlink_message)
{   
	int i;
	for(i = 0; i < REGISTERED_MISSION_MSG_CNT; i++) {
		if(mavlink_message->msgid == generic_message_list[i].msgid) {
			GENERIC_DEBUG_PRINT("%s\n\r", generic_message_list[i].name);
			generic_message_list[i].message_handler();
			return true;
		}
	}

	return false;
}

static void heartbeat_handler(mavlink_message_t *mavlink_message)
{
	//Not implement anything yet.
	return;
}

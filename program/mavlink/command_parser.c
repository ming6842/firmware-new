#include "mavlink.h"

#include "communication.h"
#include "command_parser.h"

#define REGISTERED_MISSION_MSG_CNT (sizeof(generic_message_list) / sizeof(generic_message_list[0]))

/* Debug printf */
#define USE_MAVLINK_DEBUG_PRINT 1

#if USE_MAVLINK_DEBUG_PRINT == 1
	#define MAVLINK_DEBUG_PRINT(id, name) printf("[%d]%s\n\r", id, name)
#else
	#define MAVLINK_DEBUG_PRINT(...)
#endif

/*
 * To handle a mavlink command, just create a function which follow the 
 * protocol of the mavlink and fill in the message id.
 */
struct generic_parser_data generic_message_list[] = {
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
			//MAVLINK_DEBUG_PRINT(cmd_list[i].msgid, cmd_list[i].name);
			generic_message_list[i].message_handler();
			return true;
		}
		//printf("[%d]Undefined message for onboard parser\n\r", msg->msgid);
	}

	return false;
}

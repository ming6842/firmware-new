#include "system_time.h"
#include "led.h"
static uint8_t is_led_3_on=0;

sys_time_t sys_time_manager;

sys_time_t sys_time_manager = {
	.sec = 0 ,
	.sec_remainder = 0.0f , 
	.tick_duration = SYS_TICKS_PERIOD,
	.tick_number = 0
};

void update_system_time()
{
	sys_time_manager.tick_number++;
	sys_time_manager.sec_remainder = (float)(sys_time_manager.tick_number * SYS_TICKS_PERIOD);
	if ( sys_time_manager.tick_number == SYS_TICKS_FREQUENCY){

		if(is_led_3_on == 0){
			LED_ON(LED1);
			is_led_3_on = 1;
		}
		else {
			LED_OFF(LED1);
			is_led_3_on = 0;
		}
		sys_time_manager.tick_number = 0;
		sys_time_manager.sec_remainder = 0;
		sys_time_manager.sec ++;
	}
}
uint32_t get_system_time_sec()
{
	return sys_time_manager.sec;
}
float get_system_time_sec_remainder()
{
	return sys_time_manager.sec_remainder;
}
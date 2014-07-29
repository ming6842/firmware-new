#include "system_time.h"
#include "led.h"
#include "stm32f4xx_conf.h"
#include "usart.h"
#include "io.h"

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
uint32_t get_system_time_ms()
{
	float ms = ( sys_time_manager.sec + sys_time_manager.sec_remainder ) * 1000;
	return (uint32_t)ms;
}
void display_time()
{
	uint32_t sec = get_system_time_sec();
	float float_sec_remainder = get_system_time_sec_remainder();
	uint32_t sec_remainder =(uint32_t)(1000.0f* float_sec_remainder );
	printf("%lus:%lu ms\r\n", sec, sec_remainder);

}
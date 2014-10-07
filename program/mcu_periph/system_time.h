
#ifndef FILE_SYSTEM_TIME_H
#define FILE_SYSTEM_TIME_H
//#include "stm32f4xx_conf.h"
#include <stdint.h>
#include <stdio.h>
#define SYS_TICKS_PERIOD CONTROL_DT
#define SYS_TICKS_FREQUENCY (uint32_t)CONTROL_f

typedef struct sys_time {
  volatile uint32_t sec;       //second
  volatile float sec_remainder;   //remainder of second 
  volatile uint32_t tick_number;
  const float tick_duration;
} sys_time_t;


void update_system_time(void);
uint32_t get_system_time_sec(void);
float get_system_time_sec_remainder(void);
void display_time(void);
uint32_t get_system_time_ms(void);
#endif
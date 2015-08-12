#ifndef FILE_DELAY_H
#define FILE_DELAY_H

#include <stdint.h>

#define MICRO_SECOND_TICK (configTICK_RATE_HZ / 1000000)
#define MILLI_SECOND_TICK (configTICK_RATE_HZ / 1000)

#define freertos_us_delay(us_time) vTaskDelay(us_time * (configTICK_RATE_HZ / 1000000))
#define freertos_ms_delay(ms_time) vTaskDelay(ms_time * (configTICK_RATE_HZ / 1000))

static inline void Delay_1us(uint32_t);
static inline void Delay_1us(uint32_t nCnt_1us)
{
	volatile uint32_t nCnt;

	for (; nCnt_1us != 0; nCnt_1us--)
		for (nCnt = 45; nCnt != 0; nCnt--);
}

#endif

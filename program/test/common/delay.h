#ifndef __DELAY_H
#define __DELAY_H
static inline void Delay_1us(uint32_t);
static inline void Delay_1us(uint32_t nCnt_1us)
{
	volatile uint32_t nCnt;

	for (; nCnt_1us != 0; nCnt_1us--)
		for (nCnt = 45; nCnt != 0; nCnt--);
}

#endif
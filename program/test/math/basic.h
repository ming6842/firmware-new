#ifndef __BASIC_H
#define __BASIC_H


inline int32_t bound_int32(int32_t val, int32_t min, int32_t max)
{
	return val < min ? min : (val < max ? val : max);
}


#endif
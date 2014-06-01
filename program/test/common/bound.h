#ifndef FILE_BOUND_H
#define FILE_BOUND_H

#include <stdint.h>

static inline int16_t bound_int16  (int16_t ,int16_t , int16_t );
static inline uint16_t bound_uint16(uint16_t, uint16_t , uint16_t );
static inline int32_t bound_int32  (int32_t ,int32_t , int32_t );
static inline uint32_t bound_uint32(uint32_t, uint32_t , uint32_t );
static inline int64_t bound_int64  (int64_t ,int64_t , int64_t );
static inline uint64_t bound_uint64(uint64_t, uint64_t , uint64_t );
static inline float bound_float    (float , float , float );



static inline int16_t bound_int16(int16_t val, int16_t min, int16_t max)
{
	return val < min ? min : (val > max ? max : val);
}

static inline uint16_t bound_uint16(uint16_t val, uint16_t min, uint16_t max)
{
	return val < min ? min : (val > max ? max : val);
}

static inline int32_t bound_int32(int32_t val, int32_t min, int32_t max)
{
	return val < min ? min : (val > max ? max : val);
}

static inline uint32_t bound_uint32(uint32_t val, uint32_t min, uint32_t max)
{
	return val < min ? min : (val > max ? max : val);
}

static inline int64_t bound_int64(int64_t val, int64_t min, int64_t max)
{
	return val < min ? min : (val > max ? max : val);
}

static inline uint64_t bound_uint64(uint64_t val, uint64_t min, uint64_t max)
{
	return val < min ? min : (val > max ? max : val);
}


static inline float bound_float(float val, float min, float max)
{
	return val < min ? min : (val > max ? max : val);
}


#endif
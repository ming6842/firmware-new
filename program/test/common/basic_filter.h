// basic filter


#ifndef __BASIC_FILTER_H
#define __BASIC_FILTER_H
static inline float lowpass_float(float *old, float *new, float alpha);
static inline float lowpass_float(float *old, float *new, float alpha)
{

	return (1.0f - alpha) * (*old) + alpha * (*new);

}


#endif
// basic filter


#ifndef FILE_BASIC_FILTER_H
#define FILE_BASIC_FILTER_H
static inline float lowpass_float(float *old, float *new, float alpha);
static inline float lowpass_float(float *old, float *new, float alpha)
{

	return (1.0f - alpha) * (*old) + alpha * (*new);

}


#endif
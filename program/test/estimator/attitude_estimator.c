#include "attitude_estimator.h"

inline float lowpass_float(float* old,float* new, float alpha){

	return (1.0-alpha)*(*old)+alpha*(*new);

}


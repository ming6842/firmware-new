#include "estimator.h"

void inverse_rotation_trigonometry_precal(attitude_t* attitude,euler_trigonometry_t* negative_euler){

		/* all global variable */
		negative_euler -> C_roll = arm_cos_f32(attitude->roll * (-0.01745329252392f));
		negative_euler -> S_roll = arm_sin_f32(attitude->roll * (-0.01745329252392f));

		negative_euler -> C_pitch = arm_cos_f32(attitude->pitch * (-0.01745329252392f));
		negative_euler -> S_pitch = arm_sin_f32(attitude->pitch * (-0.01745329252392f));
}
#include "attitude_estimator.h"
#include <math.h>
#include "basic_filter.h"

void attitude_estimator_init(attitude_t* attitude,imu_data_t* imu_raw_data, imu_data_t *imu_filtered_data,vector3d_f_t* True_R){

	attitude->roll=0.0;
	attitude->pitch=0.0;
	attitude->yaw=0.0;

	imu_raw_data->acc[0]=0.0;
	imu_raw_data->acc[1]=0.0;
	imu_raw_data->acc[2]=1.0;

	imu_filtered_data->acc[0] =0.0;
	imu_filtered_data->acc[1] =0.0;
	imu_filtered_data->acc[2] =1.0;

	True_R->x =0.0;
	True_R->y =0.0;
	True_R->z =1.0;
	
	estimator_trigger_flag=0;
}

void attitude_sense(attitude_t *attitude, imu_data_t *imu_raw_data, imu_data_t *imu_filtered_data, vector3d_f_t *True_R)
{

	float accel_lowpass_gain = 0.03f, gyro_lowpass_gain =0.03f,complementAlpha = 0.0001f;
	float R_raw = 0.0f, inv_R_raw = 0.0f, R_true = 0.0f, inv_R_true = 0.0f;
	float N_Ax_g = 0.0f, N_Ay_g = 0.0f, N_Az_g = 0.0f;

	float f=4000.0f;
	float dt=1.0f/f;
	float degtorad=0.01745329251994329f;
	float delta_gyroX=0.0f,delta_gyroY=0.0f,delta_gyroZ=0.0f;
	float predicted_Ax=0.0f,predicted_Ay=0.0f,predicted_Az=0.0f;


	imu_filtered_data->acc[0] = lowpass_float(&imu_filtered_data->acc[0], &imu_raw_data->acc[0], accel_lowpass_gain);
	imu_filtered_data->acc[1] = lowpass_float(&imu_filtered_data->acc[1], &imu_raw_data->acc[1], accel_lowpass_gain);
	imu_filtered_data->acc[2] = lowpass_float(&imu_filtered_data->acc[2], &imu_raw_data->acc[2], accel_lowpass_gain);
	
	imu_filtered_data->gyro[0] = lowpass_float(&imu_filtered_data->gyro[0], &imu_raw_data->gyro[0], gyro_lowpass_gain);
	imu_filtered_data->gyro[1] = lowpass_float(&imu_filtered_data->gyro[1], &imu_raw_data->gyro[1], gyro_lowpass_gain);
	imu_filtered_data->gyro[2] = lowpass_float(&imu_filtered_data->gyro[2], &imu_raw_data->gyro[2], gyro_lowpass_gain);

	R_raw = sqrtf(imu_filtered_data->acc[0] * imu_filtered_data->acc[0] + imu_filtered_data->acc[1] * imu_filtered_data->acc[1] + imu_filtered_data->acc[2] * imu_filtered_data->acc[2]);
	inv_R_raw = 1.0f / R_raw;
	N_Ax_g = imu_filtered_data->acc[0] * inv_R_raw;
	N_Ay_g = imu_filtered_data->acc[1] * inv_R_raw;
	N_Az_g = imu_filtered_data->acc[2] * inv_R_raw;

	delta_gyroX = imu_raw_data->gyro[0] * dt * degtorad;
	delta_gyroY = imu_raw_data->gyro[1] * dt * degtorad;
	delta_gyroZ = imu_raw_data->gyro[2] * dt * degtorad;


	(True_R->x) = (True_R->x) + (True_R->y) * delta_gyroZ;
	(True_R->y) =  - (True_R->x) * delta_gyroZ + (True_R->y);

	(True_R->y) = (True_R->y) + (True_R->z) * delta_gyroX;
	(True_R->z) =  - (True_R->y) * delta_gyroX + (True_R->z) ;

	(True_R->x) = (True_R->x) - (True_R->z) * delta_gyroY ;
	(True_R->z) = (True_R->x) * delta_gyroY + (True_R->z);

	predicted_Ax = (True_R->x);
	predicted_Ay = (True_R->y);
	predicted_Az = (True_R->z);


	(True_R->x) = (1.0f - complementAlpha) * (predicted_Ax) + complementAlpha * (N_Ax_g);
	(True_R->y) = (1.0f - complementAlpha) * (predicted_Ay) + complementAlpha * (N_Ay_g);
	(True_R->z) = (1.0f - complementAlpha) * (predicted_Az) + complementAlpha * (N_Az_g);


	R_true = sqrtf((True_R->x)*(True_R->x) + (True_R->y)*(True_R->y) + (True_R->z)*(True_R->z));
	inv_R_true = 1.0f/R_true;
	(True_R->x) =(True_R->x) *inv_R_true;
	(True_R->y) =(True_R->y) *inv_R_true;
	(True_R->z) =(True_R->z) *inv_R_true;


	attitude->roll=atanf(True_R->y/True_R->z)*57.2957795130823f;

	attitude->pitch=atanf(-True_R->x/sqrtf(True_R->y*True_R->y+True_R->z*True_R->z))*57.2957795130823f;



}


void heading_sense(attitude_t *attitude,imu_data_t *imu_raw_data,euler_trigonometry_t* negative_euler){

float MagXx=0.0f,MagYx=0.0f,MagZx=0.0f;
float MagX_rotated=0.0f,MagY_rotated=0.0f;//,MagZ_rotated=0.0f;

	MagXx = imu_raw_data->mag[0];
	MagYx = imu_raw_data->mag[1]*(negative_euler -> C_roll)+imu_raw_data->mag[2]*(negative_euler -> S_roll);
	MagZx = -imu_raw_data->mag[1]*(negative_euler -> S_roll)+imu_raw_data->mag[2]*(negative_euler -> C_roll);

	MagX_rotated=MagXx*(negative_euler -> C_pitch)-MagZx*(negative_euler -> S_pitch);
	MagY_rotated=MagYx;
	//MagZ_rotated=MagXx*(negative_euler -> S_pitch)+MagZx*(negative_euler -> C_pitch);

	attitude -> yaw = atan2f(-MagY_rotated,MagX_rotated)*57.32484076433121f;

	
	if((attitude -> yaw) <0.0f){

		(attitude -> yaw) += 360.0f;
	}


}


void attitude_update(attitude_t *attitude, imu_data_t *imu_filtered_data, vector3d_f_t *predicted_g_data,imu_unscaled_data_t *imu_unscaled_data,imu_data_t *imu_raw_data,imu_calibrated_offset_t *imu_offset){


		imu_update(imu_unscaled_data);
		imu_scale_data(imu_unscaled_data, imu_raw_data, imu_offset);
		#ifdef USE_MAGNETIC_HEADING
			magnetometer_update(imu_unscaled_data);
			magnetometer_scale_data(imu_unscaled_data,imu_raw_data,imu_offset);
		#endif 


		attitude_sense(attitude, imu_raw_data, imu_filtered_data, predicted_g_data);


}


#include "attitude_estimator.h"
#include <math.h>
inline float lowpass_float(float* old,float* new, float alpha){

	return (1.0-alpha)*(*old)+alpha*(*new);

}

uint8_t _buff_push[50];

void attitude_sense(attitude_t* attitude,imu_raw_data_t* imu_raw_data,vector3d_t* Acc_lp,vector3d_t* True_R){

	float accel_lowpass_gain=0.1,complementAlpha=0.00001;
	float R_raw=0.0,inv_R_raw=0.0,R_true=0.0,inv_R_true=0.0;
	float N_Ax_g=0.0, N_Ay_g=0.0, N_Az_g=0.0;

	float dt=0.000050;
	float degtorad=0.01745329251994329;
	float delta_gyroX=0.0,delta_gyroY=0.0,delta_gyroZ=0.0;
	float predicted_Ax=0.0,predicted_Ay=0.0,predicted_Az=0.0;



	Acc_lp->x = lowpass_float(&Acc_lp->x,&imu_raw_data->acc[0],accel_lowpass_gain);
	Acc_lp->y = lowpass_float(&Acc_lp->y,&imu_raw_data->acc[1],accel_lowpass_gain);
	Acc_lp->z = lowpass_float(&Acc_lp->z,&imu_raw_data->acc[2],accel_lowpass_gain);
	R_raw=sqrtf(Acc_lp->x*Acc_lp->x+Acc_lp->y*Acc_lp->y+Acc_lp->z*Acc_lp->z);
	inv_R_raw = 1.0/R_raw;
	N_Ax_g = Acc_lp->x*inv_R_raw;
	N_Ay_g = Acc_lp->y*inv_R_raw;
	N_Az_g = Acc_lp->z*inv_R_raw;

	delta_gyroX = imu_raw_data->gyro[0]*dt*degtorad;
	delta_gyroY = imu_raw_data->gyro[1]*dt*degtorad;
	delta_gyroZ = imu_raw_data->gyro[2]*dt*degtorad;


	(True_R->x) = (True_R->x) + (True_R->y) * delta_gyroZ;
	(True_R->y) =  - (True_R->x) * delta_gyroZ + (True_R->y);

	(True_R->y) = (True_R->y) + (True_R->z) * delta_gyroX;
	(True_R->z) =  - (True_R->y) * delta_gyroX + (True_R->z) ;

	(True_R->x) = (True_R->x) - (True_R->z) * delta_gyroY ;
	(True_R->z) = (True_R->x) * delta_gyroY + (True_R->z);

	predicted_Ax= (True_R->x);
	predicted_Ay= (True_R->y);
	predicted_Az= (True_R->z);


	(True_R->x) = (1.0-complementAlpha)*(predicted_Ax)+complementAlpha*(N_Ax_g);
	(True_R->y) = (1.0-complementAlpha)*(predicted_Ay)+complementAlpha*(N_Ay_g);
	(True_R->z) = (1.0-complementAlpha)*(predicted_Az)+complementAlpha*(N_Az_g);
	


	R_true = sqrtf((True_R->x)*(True_R->x) + (True_R->y)*(True_R->y) + (True_R->z)*(True_R->z));
	inv_R_true = 1.0/R_true;
	(True_R->x) =(True_R->x) *inv_R_true;
	(True_R->y) =(True_R->y) *inv_R_true;
	(True_R->z) =(True_R->z) *inv_R_true;


//float atan2f(float y, float x);
	attitude->roll=atanf(True_R->y/True_R->z)*57.2957795;

	attitude->pitch=atan(-True_R->x/sqrt(True_R->y*True_R->y+True_R->z*True_R->z))*57.2957795;


		if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET){

		 _buff_push[7]=0;
		 _buff_push[8]=0;
		 _buff_push[9]=0;
		 _buff_push[10]=0;
		 _buff_push[11]=0;
		 _buff_push[12]=0;
		 _buff_push[13]=0;

		sprintf(_buff_push,"%d,%d,%d,\r\n",(int16_t)((attitude->roll)*100.0),(int16_t)((attitude->pitch)*100.0),(int16_t)((attitude->yaw)*100.0));
		//sprintf(_buff_push,"%d,%d,%d,\r\n",(int16_t)((True_R->x)*100.0),(int16_t)((True_R->y)*100.0),(int16_t)((True_R->z)*100.0));
		//sprintf(_buff_push,"%d,%d,%d,\r\n",(int16_t)((N_Ax_g)*100.0),(int16_t)((N_Ay_g)*100.0),(int16_t)((N_Az_g)*100.0));
		//sprintf(_buff_push,"%d,%d,%d,\r\n",(int16_t)(delta_gyroX*100000.0),(int16_t)(delta_gyroY*100000.0),(int16_t)(delta_gyroZ*100000.0));
		//sprintf(_buff_push,"%d,%d,%d,\r\n",(int16_t)(predicted_Ax*100.0),(int16_t)(predicted_Ay*100.0),(int16_t)(predicted_Az*100.0));
		//sprintf(_buff_push,"%d,%d,%d,\r\n",(int16_t)(Acc_lp->x*100.0),(int16_t)(Acc_lp->y*100.0),(int16_t)(Acc_lp->z*100.0));
		
		usart2_dma_send(_buff_push);
		}

}
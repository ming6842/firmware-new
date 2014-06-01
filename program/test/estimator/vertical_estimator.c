//vertical_estimator.c

#include "vertical_estimator.h"

#include "arm_math.h"
#include <stdio.h>

void vertical_estimator_init(vertical_data* raw_data,vertical_data* filtered_data){

	raw_data->Z =0.0;
	raw_data->Zd =0.0;
	raw_data->Zdd =0.0;

	filtered_data->Z=0.0;
	filtered_data->Zd=0.0;
	filtered_data->Zdd=0.0;
}

	uint8_t _buff_push[100];

	float V_Zd_INS=0.0;
	float V_Zd_INS_error=0.0;
	float V_Zd_INS_longterm_error=0.0;
	float V_Zd_INS_longTermOffset=0.0;

	float g_offset =0.0;


void vertical_sense(vertical_data* vertical_filtered_data,vertical_data* vertical_raw_data,attitude_t* attitude,imu_raw_data_t* imu_raw_data){

	float estAlt_prev= vertical_filtered_data->Z;
	float current_Zd=0.0;
	float C_nroll=0.0,S_nroll=0.0,C_npitch=0.0,S_npitch=0.0;
	float Axx=0.0,Ayx=0.0,Azx=0.0;
	float Ax_rotated=0.0,Ay_rotated=0.0,Az_rotated=0.0;

	float V_Zd_INS_current_error;

	float g_earth = 1.0;
	float f=4000.0f;
	float dt=1.0f/f;
	/* Update barometer data */

		if(!ADS1246_DRDY_PIN_STATE()){
		vertical_raw_data->Z = barometer_read_altitude() ;
		vertical_filtered_data->Z = lowpass_float(&vertical_filtered_data->Z, &vertical_raw_data->Z, 0.005f);

		vertical_raw_data->Zd = (vertical_filtered_data->Z - estAlt_prev)*1000.0f; // Have to define it somewhere;

		}

		current_Zd =  arm_sin_f32(0.001232f);

		C_nroll = arm_cos_f32(attitude->roll * (-0.01745329252392f));
		S_nroll = arm_sin_f32(attitude->roll * (-0.01745329252392f));

		C_npitch = arm_cos_f32(attitude->pitch * (-0.01745329252392f));
		S_npitch = arm_sin_f32(attitude->pitch * (-0.01745329252392f));


		Axx = imu_raw_data->acc[0];
		Ayx = imu_raw_data->acc[1]*C_nroll+imu_raw_data->acc[2]*S_nroll;
		Azx = -imu_raw_data->acc[1]*S_nroll+imu_raw_data->acc[2]*C_nroll;

		Ax_rotated=Axx*C_npitch-Azx*S_npitch;
		Ay_rotated=Ayx;
		Az_rotated=Axx*S_npitch+Azx*C_npitch;

		vertical_filtered_data->Zdd = Az_rotated;

		/* Rotation matrix */


		/* Starting main algorithm */

		V_Zd_INS+=(Az_rotated-g_earth-g_offset )*9.81*100.0*dt;
		V_Zd_INS_error = V_Zd_INS-vertical_raw_data->Zd ;

		V_Zd_INS_current_error = vertical_filtered_data->Zd - vertical_raw_data->Zd;
		V_Zd_INS_longterm_error = lowpass_float(&V_Zd_INS_longterm_error,&V_Zd_INS_current_error,0.000001f); // may not be long term enough for new one


		V_Zd_INS_longTermOffset+=V_Zd_INS_longterm_error*1.0f*100.5f;


		if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET) {

			_buff_push[7] = 0;_buff_push[8] = 0;_buff_push[9] = 0;_buff_push[10] = 0;_buff_push[11] = 0;_buff_push[12] = 0;	_buff_push[13] = 0;

			sprintf((char *)_buff_push, "%d,%d,%d,100000000000,\r\n",
				(int16_t)(Ax_rotated * 100.0f),
				(int16_t)(Ay_rotated * 100.0f),
				(int16_t)(Az_rotated * 100.0f));

			usart2_dma_send(_buff_push);

		}	

}



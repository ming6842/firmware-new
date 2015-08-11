//vertical_estimator.c

#include "vertical_estimator.h"



//#define VZD_DEBUGGING

#ifdef VZD_DEBUGGING
#include <stdio.h>
#include "usart.h"
	uint8_t _buff_push[100];
    
#define PUSH_PRESC 100
    uint32_t push_prescaler = PUSH_PRESC;
#endif


void vertical_estimator_init(vertical_data_t* raw_data,vertical_data_t* filtered_data){

	raw_data->Z =0.0;
	raw_data->Zd =0.0;
	raw_data->Zdd =0.0;

	filtered_data->Z=0.0;
	filtered_data->Zd=0.0;
	filtered_data->Zdd=0.0;
}

	float V_Z_Baro_lp = 0.0f;
	float V_Zd_INS=0.0;
	float V_Zd_INS_error=0.0;
	float V_Zd_INS_longterm_error=0.0;
	float V_Zd_INS_longTermOffset=0.0;
	float V_Zdd_INS_error=0.0;
	//float V_Zd_INS_beforeCorrection;

	float V_Zd_Baro_Prev=0.0;
	float V_Zd_INS_error_Prev=0.0;

	float g_offset =-0.005f;


void vertical_sense(vertical_data_t* vertical_filtered_data,vertical_data_t* vertical_raw_data,imu_data_t* imu_raw_data, euler_trigonometry_t* negative_euler){

	float estAlt_prev= V_Z_Baro_lp;
	float Axx=0.0,Azx=0.0; //,Ayx=0.0
	float Az_rotated;//Ax_rotated=0.0,Ay_rotated,

	float V_Zd_INS_current_error;
	float V_Zdd_INS_current_error;
	float g_earth = 1.0f;
	float del_g_adder;
	float f=4000.0f;
	float dt=1.0f/f;


	float Z_INS_Alpha=0.00004f,Zd_INS_Alpha=0.0001f,Zdd_INS_error_Alpha=0.005f;
	/* Update barometer data */

	#ifndef USE_CAN_ADS1246_MPX6115A 
		if(!ADS1246_DRDY_PIN_STATE()){
	#else

		if( CAN2_CheckMessageStatusFlag(CAN_MESSAGE_BAROMETER) == 1){

	#endif

		vertical_raw_data->Z = barometer_read_altitude() ;
		V_Z_Baro_lp= lowpass_float(&V_Z_Baro_lp, &vertical_raw_data->Z, 0.005f);

		vertical_raw_data->Zd = (V_Z_Baro_lp- estAlt_prev)*1000.0f; // Have to define it somewhere;

		}

		vertical_filtered_data->Z += vertical_filtered_data->Zd*CONTROL_DT;

		if (abs((int32_t)((vertical_filtered_data->Z) - V_Z_Baro_lp))<(int32_t)300){
		vertical_filtered_data->Z = V_Z_Baro_lp * Z_INS_Alpha + vertical_filtered_data->Z  * (1.0f-  Z_INS_Alpha);
		}else{
		
		vertical_filtered_data->Z = V_Z_Baro_lp ;
		

		}


		/* S/(negative_euler -> C_roll)/pitch will now be calculated from estimator.c precal function*/

		Axx = imu_raw_data->acc[0];
		//Ayx = imu_raw_data->acc[1]*(negative_euler -> C_roll)+imu_raw_data->acc[2]*(negative_euler -> S_roll);
		Azx = -imu_raw_data->acc[1]*(negative_euler -> S_roll)+imu_raw_data->acc[2]*(negative_euler -> C_roll);

		//Ax_rotated=Axx*(negative_euler -> C_pitch)-Azx*(negative_euler -> S_pitch);
		//Ay_rotated=Ayx;
		Az_rotated=Axx*(negative_euler -> S_pitch)+Azx*(negative_euler -> C_pitch);

		vertical_filtered_data->Zdd = Az_rotated;

		/* Rotation matrix */


		/* Starting main algorithm */

		V_Zd_INS+=(Az_rotated-g_earth-g_offset )*9.81f*100.0f*dt;
		V_Zd_INS_error = V_Zd_INS-vertical_raw_data->Zd ;


		/* estimate longterm error haven't been corrected */
		V_Zd_INS_current_error = vertical_filtered_data->Zd - vertical_raw_data->Zd;
		V_Zd_INS_longterm_error = lowpass_float(&V_Zd_INS_longterm_error,&V_Zd_INS_current_error,0.00000001f); // may not be long term enough for new one

		/* apply longterm error correction */
		V_Zd_INS_longTermOffset+=V_Zd_INS_longterm_error*1.0f*1000.5f;
		/* substract applied longterm error from bank */
		V_Zd_INS_longterm_error-=V_Zd_INS_longterm_error*0.210f;
		vertical_filtered_data->Zd =V_Zd_INS-V_Zd_INS_longTermOffset;

		/* estimate the stray acceleration with lowpass filter */
		V_Zdd_INS_current_error = (V_Zd_INS_error-V_Zd_INS_error_Prev)*f*0.01f;
		V_Zdd_INS_error = lowpass_float(&V_Zdd_INS_error,&V_Zdd_INS_current_error,Zdd_INS_error_Alpha);
		
		/* calibrating earth's gravity offset */
		del_g_adder=0.00005f*V_Zdd_INS_error*0.1f;

		g_offset+=del_g_adder;


		/* complementary filter for integrity */
		//V_Zd_INS_beforeCorrection=V_Zd_INS;
		vertical_filtered_data->Zd = vertical_raw_data->Zd * Zd_INS_Alpha+(1.0f-Zd_INS_Alpha)*vertical_filtered_data->Zd;

		/* memory storage for next iteration */
		//V_Zd_INS_correctionOffset=V_Zd_INS-V_Zd_INS_beforeCorrection;
		V_Zd_Baro_Prev=vertical_raw_data->Zd;
		V_Zd_INS_error_Prev=V_Zd_INS_error;

#ifdef VZD_DEBUGGING

		if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET) {

			if((push_prescaler--) == 0){
			_buff_push[7] = 0;_buff_push[8] = 0;_buff_push[9] = 0;_buff_push[10] = 0;_buff_push[11] = 0;_buff_push[12] = 0;	_buff_push[13] = 0;

			sprintf((char *)_buff_push, "%ld,%ld,%ld,%d,\r\n",

				// (int32_t)(V_Z_Baro_lp* 1.0f),
				// (int32_t)(vertical_raw_data->Z* 1.0f),
				(int32_t)(vertical_filtered_data->Z* 1.0f),
				(int32_t)(vertical_filtered_data->Zd * 1.0f),
				(int32_t)(g_offset * 100000.0f),
				(int16_t)(V_Zd_INS * 1.0f));

			// usart2_dma_send(_buff_push);

			push_prescaler = PUSH_PRESC;
			}
		}	
#endif
}



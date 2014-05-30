

#define USE_IMU_MPU9250

#include "stm32f4xx_conf.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "imu.h"
#include <stdio.h>
#include "attitude_estimator.h"

extern uint8_t estimator_trigger_flag;


void Delay_1us(uint32_t nCnt_1us)
{
	volatile uint32_t nCnt;

	for (; nCnt_1us != 0; nCnt_1us--)
		for (nCnt = 45; nCnt != 0; nCnt--);
}
int main(void)
{
	uint8_t buffer[100];

	imu_unscaled_data_t imu_unscaled_data;
	imu_raw_data_t imu_raw_data;
	imu_calibrated_offset_t imu_offset;
	attitude_t attitude;
	vector3d_t lowpassed_acc_data;
	vector3d_t predicted_g_data;

	predicted_g_data.x=0.0;
	predicted_g_data.y=0.0;
	predicted_g_data.z=1.0;

	estimator_trigger_flag=0;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,  ENABLE);
	led_init();
	usart_init();
	spi_init();
	pwm_input_output_init();
	i2c_Init();
	usart2_dma_init();

	//Delay_1us(2000000);

	imu_initialize();

	//Delay_1us(100000);
	imu_calibrate_gyro_offset(&imu_offset,15000);
	sprintf( (char *)buffer,"%d,%d,%d,\r\n",(int16_t)(imu_offset.gyro[0]),(int16_t)(imu_offset.gyro[1]),(int16_t)(imu_offset.gyro[2]));
	usart2_dma_send(buffer);

	//Delay_1us(10000);

	while(1) {

		if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET){

		 buffer[7]=0;
		 buffer[8]=0;
		 buffer[9]=0;
		 buffer[10]=0;
		 buffer[11]=0;
		 buffer[12]=0;
		 buffer[13]=0;
		//sprintf(buffer,"%d,%d,%d,\r\n",(int16_t)(imu_raw_data.gyro[0]*10.0),(int16_t)(imu_raw_data.gyro[1]*10.0),(int16_t)(imu_raw_data.gyro[2]*10.0));
		//sprintf(buffer,"%d,%d,%d,\r\n",(int16_t)(imu_raw_data.acc[0]*100.0),(int16_t)(imu_raw_data.acc[1]*100.0),(int16_t)(imu_raw_data.acc[2]*100.0));
		//sprintf(buffer,"%d,%d,%d,\r\n",(int16_t)(lowpassed_acc_data.x*100.0),(int16_t)(lowpassed_acc_data.y*100.0),(int16_t)(lowpassed_acc_data.z*100.0));
		//sprintf(buffer,"%d,%d,%d,\r\n",(int16_t)(predicted_g_data.x*100.0),(int16_t)(predicted_g_data.y*100.0),(int16_t)(predicted_g_data.z*100.0));
		sprintf((char *)buffer,"%d,%d,%d,\r\n",
			(int16_t) (attitude.roll*100.0f),
			(int16_t) (attitude.pitch*100.0f),
			(int16_t) (attitude.yaw*100.0f) );
		
		//sprintf(buffer,"%d,\r\n",(int16_t)(imu_raw_data.temp*100.0));
		
		usart2_dma_send(buffer);

		}


	imu_update(&imu_unscaled_data);
	imu_scale_data(&imu_unscaled_data, &imu_raw_data,&imu_offset);
	attitude_sense(&attitude,&imu_raw_data,&lowpassed_acc_data,&predicted_g_data);

		GPIO_ToggleBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15);
//		Delay_1us(1000);

		while(estimator_trigger_flag==0);
		estimator_trigger_flag=0;
	}

	return 0;
}




#define USE_IMU_MPU9250
#define USE_ADS1246_MPX6115A

#include "stm32f4xx_conf.h"
#include "../common/delay.h"
#include "gpio.h"
#include "led.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include <stdio.h>
#include "attitude_estimator.h"
#include "vertical_estimator.h"
#include "input_capture.h"
#include "pwm.h"
extern uint8_t estimator_trigger_flag;



int main(void)
{
	uint8_t buffer[100];
	imu_unscaled_data_t imu_unscaled_data;
	imu_raw_data_t imu_raw_data;
	imu_calibrated_offset_t imu_offset;
	attitude_t attitude;
	vector3d_t lowpassed_acc_data;
	vector3d_t predicted_g_data;

	vertical_data vertical_raw_data;
	vertical_data vertical_filtered_data;



	attitude_estimator_init(&attitude,&imu_raw_data, &lowpassed_acc_data,&predicted_g_data);
	vertical_estimator_init(&vertical_raw_data,&vertical_filtered_data);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,  ENABLE);
	led_init();
	usart_init();
	spi_init();
	pwm_input_output_init();
	i2c_Init();
	usart2_dma_init();

	//Delay_1us(2000000);

	imu_initialize();

	//Delay_1us(100000);
	imu_calibrate_gyro_offset(&imu_offset, 15000);
	//ads1246_initialize();

	while (1) {

		if (DMA_GetFlagStatus(DMA1_Stream6, DMA_FLAG_TCIF6) != RESET) {

			buffer[7] = 0;buffer[8] = 0;buffer[9] = 0;buffer[10] = 0;buffer[11] = 0;buffer[12] = 0;	buffer[13] = 0;
			sprintf((char *)buffer, "%ld,%ld,%ld,%ld,%ld,%ld\r\n",
				inc[INC1].curr_value,
				inc[INC2].curr_value,
				inc[INC3].curr_value,
				inc[INC4].curr_value,
				inc[INC5].curr_value,
				inc[INC6].curr_value);


			usart2_dma_send(buffer);

		}	


		imu_update(&imu_unscaled_data);
		imu_scale_data(&imu_unscaled_data, &imu_raw_data, &imu_offset);
		attitude_sense(&attitude, &imu_raw_data, &lowpassed_acc_data, &predicted_g_data);

		vertical_sense(&vertical_filtered_data,&vertical_raw_data,&attitude);

		while(estimator_trigger_flag==0);
		estimator_trigger_flag=0;
	}

	return 0;
}




#define USE_IMU_MPU9250

#include "stm32f4xx_conf.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "mpu9250.h"
#include <stdio.h>
#include "imu.h"



void Delay_1us(vu32 nCnt_1us)
{
	u32 nCnt;

	for (; nCnt_1us != 0; nCnt_1us--)
		for (nCnt = 45; nCnt != 0; nCnt--);
}
	uint8_t rcv_id;

int main(void)
{

	imu_unscaled_data_t mpu9250_unscaled_data;
	imu_raw_data_t mpu9250_raw_data;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,  ENABLE);
	led_init();
	usart_init();
	spi_init();
	pwm_input_output_init();
	i2c_Init();
	uint8_t rxdata;
	uint8_t i2c1_data;
	
	Delay_1us(10000);

	imu_initialize();
	while(1) {
		usart_dma_send("test dma\r\n");
//	mpu9250_write_byte(0x1A,0x02);
//	Delay_1us(10);
//	rxdata = mpu9250_read_byte(0x3B);

//	USART_SendData(UART8,rxdata);

//	mpu9250_read_accel_temp_gyro(&mpu9250_unscaled_data);
	imu_update(&mpu9250_unscaled_data);
	mpu9250_convert_to_scale(&mpu9250_unscaled_data, &mpu9250_raw_data);

//	printf("%f,%f,%f,\r\n",mpu9250_raw_data.acc[1],mpu9250_raw_data.acc[2],mpu9250_raw_data.acc[3]);
	printf("%f,%f,%f,\r\n",mpu9250_raw_data.gyro[1],mpu9250_raw_data.gyro[2],mpu9250_raw_data.gyro[3]);
//	printf("%d,%d,%d,",mpu9250_unscaled_data.acc[1],mpu9250_unscaled_data.acc[2],mpu9250_unscaled_data.acc[3]);
//	printf("%d,%d,%d,",mpu9250_unscaled_data.gyro[1],mpu9250_unscaled_data.gyro[2],mpu9250_unscaled_data.gyro[3]);
//	printf("%d\r\n",mpu9250_unscaled_data.temp);
	//rxdata = mpu9250_read_who_am_i();
//	Delay_1us(10);


		GPIO_ToggleBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15);
//		Delay_1us(1000);
	}

	return 0;
}


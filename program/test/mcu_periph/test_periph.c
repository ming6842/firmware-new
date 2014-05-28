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
	mpu9250_reset();



	while(1) {
	printf("071071071 yoyoyoy\r\n ");
//	mpu9250_write_byte(0x1A,0x02);
	Delay_1us(10);
	rxdata = mpu9250_read_byte(0x3B);

	USART_SendData(UART8,rxdata);

	mpu9250_read_accel_temp_gyro(&mpu9250_raw_data);
	//rxdata = mpu9250_read_who_am_i();
	Delay_1us(10);


		GPIO_ToggleBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15);
		Delay_1us(100000);
	}

	return 0;
}


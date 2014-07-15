

#define USE_IMU_MPU9250
#define SLAVE_ADDRESS 0x68 << 1  // the slave address for MPU6050 Who am I

#define  EEP_Firstpage 0x5e


#include "stm32f4xx_conf.h"
#include "gpio.h"
#include "i2c.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "imu.h"
#include <stdio.h>
#include "attitude_estimator.h"
#include "AT24C04C.h"
#include "MPU6050_Read_WHO_AM_I.h"



void Delay_1us(uint32_t nCnt_1us)
{
	volatile uint32_t nCnt;

	for (; nCnt_1us != 0; nCnt_1us--)
		for (nCnt = 45; nCnt != 0; nCnt--);
}

// void I2C_Test(void)
// {
// 	uint16_t i;
// 	uint8_t I2c_Buf_Write[1];
// 	uint8_t I2c_Buf_Read[1];

// 	printf("Write data\n\r");
    
// 	// for ( i=0; i<=255; i++ ) //填充缓冲
//  //  {   
//     I2c_Buf_Write[0] = 13;
//    //  printf("0x%02X ", I2c_Buf_Write[i]);
//    //  if(i%16 == 15)    
//    //      printf("\n\r");    
//    // }

//   //将I2c_Buf_Write中顺序递增的数据写入EERPOM中 
// 	I2C_EE_BufferWrite( I2c_Buf_Write, EEP_Firstpage, 1);	 
  
//   printf("\n\rRead data\n\r");
//   //将EEPROM读出数据顺序保持到I2c_Buf_Read中 
// 	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 1); 

//  //  //将I2c_Buf_Read中的数据通过串口打印
// 	// for (i=0; i<256; i++)
// 	// {	
// 	// 	if(I2c_Buf_Read[i] != I2c_Buf_Write[i])
// 	// 	{
// 	// 		printf("0x%02X ", I2c_Buf_Read[i]);
// 	// 		printf("Error:I2C EEPROM write data is different with read data\n\r");
// 	// 		return;
// 	// 	}
//  //    printf("0x%02X ", I2c_Buf_Read[i]);
//  //    if(i%16 == 15)    
//  //        printf("\n\r");
    
// 	// }
//   printf("I2C(AT24C02) success\n\r");
// }

void I2C_Test()
{
	uint16_t i;
	uint8_t I2c_Buf_Write[1];
	uint8_t I2c_Buf_Read[1];

    I2c_Buf_Write[0] = 2;

	I2C_EE_PageWrite(I2c_Buf_Write, EEP_Firstpage, 1);
	I2C_EE_WaitEepromStandbyState();

	I2C_EE_BufferRead(I2c_Buf_Read, EEP_Firstpage, 1);

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

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE,  ENABLE);
	led_init();
	usart_init();
	spi_init();
	pwm_input_output_init();
	
	usart2_dma_init();

	//Delay_1us(2000000);

	imu_initialize();

	//Delay_1us(100000);
	imu_calibrate_gyro_offset(&imu_offset,15000);
	sprintf( (char *)buffer,"%d,%d,%d,\r\n",(int16_t)(imu_offset.gyro[0]),(int16_t)(imu_offset.gyro[1]),(int16_t)(imu_offset.gyro[2]));
	usart2_dma_send(buffer);
	

	//Delay_1us(10000);
	i2c_Init();

	//eeprom_byte_write(I2C1, SLAVE_ADDRESS1, RegisterAddr, data);
	
	//a = eeprom_byte_read(I2C1, SLAVE_ADDRESS2, RegisterAddr);

    I2C_Test();

    Delay_1us(10000);

	while(1) {

		

		//Delay_1us(1000);


	// 	if(DMA_GetFlagStatus(DMA1_Stream6,DMA_FLAG_TCIF6)!=RESET){

	// 	 buffer[7]=0;
	// 	 buffer[8]=0;
	// 	 buffer[9]=0;
	// 	 buffer[10]=0;
	// 	 buffer[11]=0;
	// 	 buffer[12]=0;
	// 	 buffer[13]=0;
	// 	//sprintf(buffer,"%d,%d,%d,\r\n",(int16_t)(imu_raw_data.gyro[0]*10.0),(int16_t)(imu_raw_data.gyro[1]*10.0),(int16_t)(imu_raw_data.gyro[2]*10.0));
	// 	//sprintf(buffer,"%d,%d,%d,\r\n",(int16_t)(imu_raw_data.acc[0]*100.0),(int16_t)(imu_raw_data.acc[1]*100.0),(int16_t)(imu_raw_data.acc[2]*100.0));
	// 	//sprintf(buffer,"%d,%d,%d,\r\n",(int16_t)(lowpassed_acc_data.x*100.0),(int16_t)(lowpassed_acc_data.y*100.0),(int16_t)(lowpassed_acc_data.z*100.0));
	// 	//sprintf(buffer,"%d,%d,%d,\r\n",(int16_t)(predicted_g_data.x*100.0),(int16_t)(predicted_g_data.y*100.0),(int16_t)(predicted_g_data.z*100.0));
	// 	sprintf((char *)buffer,"%d,%d,%d,\r\n",
	// 		(int16_t) (attitude.roll*100.0f),
	// 		(int16_t) (attitude.pitch*100.0f),
	// 		(int16_t) (attitude.yaw*100.0f) );
		
	// 	//sprintf(buffer,"%d,\r\n",(int16_t)(imu_raw_data.temp*100.0));
		
	// 	usart2_dma_send(buffer);

	// 	}


	// imu_update(&imu_unscaled_data);
	// imu_scale_data(&imu_unscaled_data, &imu_raw_data,&imu_offset);
	// attitude_sense(&attitude,&imu_raw_data,&lowpassed_acc_data,&predicted_g_data);

	// 	GPIO_ToggleBits(GPIOE, GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_15);
	// 	//Delay_1us(1000);
	}

	return 0;
}


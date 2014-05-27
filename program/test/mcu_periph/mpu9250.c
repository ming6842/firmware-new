#include "stm32f4xx_conf.h"
#include "mpu9250.h"
#include "spi.h"



#define MPU9250_SPI SPI4
#define MPU9250_SELECT() 	GPIO_ResetBits(GPIOE,GPIO_Pin_4)
#define MPU9250_DESELECT() 	GPIO_SetBits(GPIOE,GPIO_Pin_4)

void mpu9250_delay(uint32_t count){

	while(count--){

	}
}

uint8_t mpu9250_read_byte(uint8_t addr)
{
	uint8_t rxData;

	MPU9250_SELECT();

					 SPI_xfer(MPU9250_SPI,addr | 0x80);
			rxData = SPI_xfer(MPU9250_SPI,0xff);
	MPU9250_DESELECT();

return rxData;
}
uint8_t mpu9250_read_who_am_i()
{
	uint8_t rcv_id;
	rcv_id = mpu9250_read_byte(0x75);

return rcv_id;
}

void mpu9250_write_byte(uint8_t addr,uint8_t data){

	MPU9250_SELECT();

					 SPI_xfer(MPU9250_SPI,addr);
					 SPI_xfer(MPU9250_SPI,data);
	MPU9250_DESELECT();

}

void mpu9250_reset(){

	mpu9250_write_byte(0x6B,0x80);

	mpu9250_delay(100);
}
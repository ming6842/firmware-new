#include "hmc5983.h"

#include "usart.h"
#include <stdio.h>
#include "led.h"


void hmc5983_delay(volatile uint32_t count)
{

	while (count--) {

	}
}

uint8_t hmc5983_readByte(uint8_t addr){

	uint8_t data ;
	HMC5983_SELECT();
	SPI_xfer(HMC5983_SPI , addr| 0x80);
	data = SPI_xfer(HMC5983_SPI , 0x00);
	HMC5983_DESELECT();

	return data;
}



void hmc5983_initialize_config(void){

	HMC5983_SELECT();
	/* Writing configuration register A */
	SPI_xfer(HMC5983_SPI , HMC5983_CONFIG_REG_A| 0x00);
	/* Enabled temperature sensor, 8-sample averaged, 220Hz ODR */
	SPI_xfer(HMC5983_SPI , 0xFC);
	HMC5983_DESELECT();
	hmc5983_delay(100);

	HMC5983_SELECT();
	/* Writing configuration register B */
	SPI_xfer(HMC5983_SPI , HMC5983_CONFIG_REG_B| 0x00);
	/* Highest gain setting */
	SPI_xfer(HMC5983_SPI , 0x00);
	HMC5983_DESELECT();
	hmc5983_delay(100);

	HMC5983_SELECT();
	/* Writing configuration register B */
	SPI_xfer(HMC5983_SPI , HMC5983_MODE_REG| 0x00);
	/* Highest gain setting */
	SPI_xfer(HMC5983_SPI , 0x00);
	HMC5983_DESELECT();
	hmc5983_delay(100);

}


void hmc5983_update(imu_unscaled_data_t *imu_unscaledData){

 uint8_t hmc5983_buffer[6];

 	HMC5983_SELECT();
 	SPI_xfer(HMC5983_SPI , HMC5983_DATA_OUTOUT_X_H | 0xC0);
	hmc5983_buffer[0] = SPI_xfer(HMC5983_SPI , 0x00);
	hmc5983_buffer[1] = SPI_xfer(HMC5983_SPI , 0x00);
	hmc5983_buffer[2] = SPI_xfer(HMC5983_SPI , 0x00);
	hmc5983_buffer[3] = SPI_xfer(HMC5983_SPI , 0x00);
	hmc5983_buffer[4] = SPI_xfer(HMC5983_SPI , 0x00);
	hmc5983_buffer[5] = SPI_xfer(HMC5983_SPI , 0x00);

	HMC5983_DESELECT();

	imu_unscaledData->mag[0] = ((uint16_t)hmc5983_buffer[0] << 8) | (uint16_t)hmc5983_buffer[1];
	imu_unscaledData->mag[2] = ((uint16_t)hmc5983_buffer[2] << 8) | (uint16_t)hmc5983_buffer[3];
	imu_unscaledData->mag[1] = ((uint16_t)hmc5983_buffer[4] << 8) | (uint16_t)hmc5983_buffer[5];



}
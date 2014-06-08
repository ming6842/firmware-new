#include "hmc5983.h"



void hmc5983_delay(volatile uint32_t count)
{

	while (count--) {

	}
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

void hmc5983_update(void){

// uint8_t hmc5983_buffer[6];

// 	HMC5983_SELECT();
// 	SPI_xfer(HMC5983_SPI , HMC5983_DATA_OUTOUT_X_H | 0xC0);
// 	hmc5983_buffer[0] = SPI_xfer(HMC5983_SPI , 0x00);
// 	hmc5983_buffer[1] = SPI_xfer(HMC5983_SPI , 0x00);
// 	hmc5983_buffer[2] = SPI_xfer(HMC5983_SPI , 0x00);
// 	hmc5983_buffer[3] = SPI_xfer(HMC5983_SPI , 0x00);
// 	hmc5983_buffer[4] = SPI_xfer(HMC5983_SPI , 0x00);
// 	hmc5983_buffer[5] = SPI_xfer(HMC5983_SPI , 0x00);

	HMC5983_DESELECT();


}
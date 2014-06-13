#include "stm32f4xx_conf.h"
#include "mpu9250.h"
#include "led.h"



void mpu9250_delay(volatile uint32_t count)
{

	while (count--) {

	}
}

uint8_t mpu9250_read_byte(uint8_t addr)
{
	uint8_t rxData;

	MPU9250_SELECT();

	SPI_xfer(MPU9250_SPI, addr | 0x80);
	rxData = SPI_xfer(MPU9250_SPI, 0xff);
	MPU9250_DESELECT();

	return rxData;
}
uint8_t mpu9250_read_who_am_i()
{
	uint8_t rcv_id;
	rcv_id = mpu9250_read_byte(MPU9250_WHO_AM_I);

	return rcv_id;
}

void mpu9250_write_byte(uint8_t addr, uint8_t data)
{

	MPU9250_SELECT();

	SPI_xfer(MPU9250_SPI, addr);
	SPI_xfer(MPU9250_SPI, data);
	MPU9250_DESELECT();

}

void mpu9250_reset()
{

	mpu9250_write_byte(MPU9250_PWR_MGMT_1, 0x80); //Reset command = 0x80
	mpu9250_delay(1000000);
}

void mpu9250_initialize_config()
{
	mpu9250_reset(); // reset chip and wait
	mpu9250_delay(1000000);
	mpu9250_write_byte(MPU9250_GYRO_CONFIG, 0x10); // 0x10 => Full scale 1000Hz
	mpu9250_delay(1000000);
	mpu9250_write_byte(MPU9250_ACCEL_CONFIG, 0x10); // 0x10 => Full scale 8g
	mpu9250_delay(1000000);


}

void mpu9250_calibrate_gyro_offset(imu_calibrated_offset_t *imu_offset, uint16_t count)
{

	imu_unscaled_data_t mpu9250_cache_unscaled_data;
	imu_data_t mpu9250_cache_average_data;

	mpu9250_cache_average_data.gyro[0] = 0.0;
	mpu9250_cache_average_data.gyro[1] = 0.0;
	mpu9250_cache_average_data.gyro[2] = 0.0;
	uint16_t i = 0;

	for (i = 0; i < count; i++) {

		mpu9250_read_accel_temp_gyro(&mpu9250_cache_unscaled_data);
		mpu9250_cache_average_data.gyro[0] += ((float)mpu9250_cache_unscaled_data.gyro[0]) / (float)count;
		mpu9250_cache_average_data.gyro[1] += ((float)mpu9250_cache_unscaled_data.gyro[1]) / (float)count;
		mpu9250_cache_average_data.gyro[2] += ((float)mpu9250_cache_unscaled_data.gyro[2]) / (float)count;

		mpu9250_delay(100);
		LED_TOGGLE(LED2);
	}

	imu_offset->gyro[0] = (int16_t)mpu9250_cache_average_data.gyro[0];
	imu_offset->gyro[1] = (int16_t)mpu9250_cache_average_data.gyro[1];
	imu_offset->gyro[2] = (int16_t)mpu9250_cache_average_data.gyro[2];

}

void mpu9250_apply_accel_calibration(imu_calibrated_offset_t *imu_offset){

	/* Example of data for current board

	Raw_Axis |  min   | max  |  average(offset) | 1g_scale	|>

	    X	   -4114	4060	-27					4087 //4094 (fine calibrated)
	    Y	   -4091	4092    0 					4091 // 4095 (fine calibrated)
	    Z	   -4547  	3712	417.5				4129 //4xxx (fine calibrated)
	
	But actual raw_data for 1g in 8g_full_scale setting should be 4096
	So that the modify factor for acc_scale will be 4096/(measured1g_scale) (i.e. scale it to 4096)
	*/

	imu_offset->acc[0]=-27;
	imu_offset->acc[1]=0;
	imu_offset->acc[2]=-417;

	imu_offset->acc_scale[0]=4096.0f/4094.0f;//1.002202104f;
	imu_offset->acc_scale[1]=4096.0f/4095.0f;//1.001222195f;
	imu_offset->acc_scale[2]=4096.0f/4129.0f;//0.992007750f;

}

void mpu9250_read_accel_temp_gyro(imu_unscaled_data_t *imu_unscaledData)
{

	uint8_t buffer[14];

	MPU9250_SELECT();

	SPI_xfer(MPU9250_SPI, MPU9250_ACCEL_XOUT_H | 0x80);
	buffer[0] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[1] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[2] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[3] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[4] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[5] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[6] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[7] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[8] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[9] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[10] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[11] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[12] =  SPI_xfer(MPU9250_SPI, 0xff);
	buffer[13] =  SPI_xfer(MPU9250_SPI, 0xff);

	imu_unscaledData->acc[0] = ((uint16_t)buffer[0] << 8) | (uint16_t)buffer[1];
	imu_unscaledData->acc[1] = ((uint16_t)buffer[2] << 8) | (uint16_t)buffer[3];
	imu_unscaledData->acc[2] = ((uint16_t)buffer[4] << 8) | (uint16_t)buffer[5];

	imu_unscaledData->temp = ((uint16_t)buffer[6] << 8) | (uint16_t)buffer[7];

	imu_unscaledData->gyro[0] = ((uint16_t)buffer[8] << 8) | (uint16_t)buffer[9];
	imu_unscaledData->gyro[1] = ((uint16_t)buffer[10] << 8) | (uint16_t)buffer[11];
	imu_unscaledData->gyro[2] = ((uint16_t)buffer[12] << 8) | (uint16_t)buffer[13];


	MPU9250_DESELECT();


}

void mpu9250_convert_to_scale(imu_unscaled_data_t *imu_unscaledData, imu_data_t *imu_scaledData, imu_calibrated_offset_t *imu_offset)
{

	imu_scaledData->acc[0]	= (float)(imu_unscaledData->acc[0]-imu_offset->acc[0]) * MPU9250A_8g * imu_offset->acc_scale[0];
	imu_scaledData->acc[1]	= -(float)(imu_unscaledData->acc[1]-imu_offset->acc[1]) * MPU9250A_8g * imu_offset->acc_scale[1]; // correct with board orientation
	imu_scaledData->acc[2]	= (float)(imu_unscaledData->acc[2]-imu_offset->acc[2]) * MPU9250A_8g * imu_offset->acc_scale[2];

	imu_scaledData->gyro[0]	= -(float)(imu_unscaledData->gyro[0] - imu_offset->gyro[0]) * MPU9250G_1000dps; // correct with board orientation
	imu_scaledData->gyro[1]	= (float)(imu_unscaledData->gyro[1] - imu_offset->gyro[1]) * MPU9250G_1000dps;
	imu_scaledData->gyro[2]	= -(float)(imu_unscaledData->gyro[2] - imu_offset->gyro[2]) * MPU9250G_1000dps; // correct with board orientation

	imu_scaledData->temp = ((float)(imu_unscaledData->temp) * MPU9250T_85degC + 21.0f);


}

void mpu9250_initialize_system(imu_calibrated_offset_t *imu_offset,uint16_t count){

	mpu9250_initialize_config();
	mpu9250_calibrate_gyro_offset(imu_offset,count); 
	// // for debugger purpose
	// imu_offset->gyro[0]=(int16_t)8;
	// imu_offset->gyro[1]=(int16_t)86;
	// imu_offset->gyro[2]=(int16_t)(-23);
	mpu9250_apply_accel_calibration(imu_offset);

}
// vertical_estimator.h
#ifndef __VERTICAL_ESTMATOR_H
#define __VERTICAL_ESTMATOR_H


#ifdef USE_ADS1246_MPX6115A

	
// #define barometer_read_altitude(rawdata) mpu9250_read_accel_temp_gyro(rawdata)
// #define barometer_initialize() mpu9250_initialize_config()
// #define barometer_scale_data(rawdata,scaled_data,offset) mpu9250_convert_to_scale(rawdata, scaled_data,offset)
// #define barometer_get_tare_value(offset,count) mpu9250_calibrate_gyro_offset(offset,count)

#endif
typedef struct vertical_data
{
	float Z =0.0;
	float Zd =0.0;
	float Zdd =0.0;

};

#endif
// barometer.h


#ifndef FILE_BAROMETER_H
#define FILE_BAROMETER_H

#include "ADS1246_MPX6115A.h"


//#pragma message "USE_ADS1246_MPX6115A" USE_ADS1246_MPX6115A
#ifdef USE_ADS1246_MPX6115A

	#ifndef USE_CAN_ADS1246_MPX6115A 

		#define barometer_read_altitude() MPX6115_get_raw_altitude(ads1246_readADCconversion())
		#define barometer_initialize() ads1246_initialize()
		#define barometer_update_tare_value() ;


	#else

		#define barometer_read_altitude() MPX6115_get_raw_altitude(ads1246_CAN_UpdateADC())
		#define barometer_initialize() MPX6115_CAN_Initialize()
		#define barometer_update_tare_value() MPX6115_CAN_update_tare_value()


	#endif

#endif


#endif
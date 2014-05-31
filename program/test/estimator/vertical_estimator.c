//vertical_estimator.c

#include "vertical_estimator.h"

void vertical_estimator_init(vertical_data* raw_data,vertical_data* filtered_data){

	raw_data->Z =0.0;
	raw_data->Zd =0.0;
	raw_data->Zdd =0.0;

	filtered_data->Z=0.0;
	filtered_data->Zd=0.0;
	filtered_data->Zdd=0.0;
}

void vertical_sense(vertical_data* vertical_filtered_data,vertical_data* vertical_raw_data,attitude_t* attitude){

	/* Update barometer data */

		if(!ADS1246_DRDY_PIN_STATE()){
		vertical_raw_data->Z = barometer_read_altitude() ;
		vertical_filtered_data->Z = lowpass_float(&vertical_filtered_data->Z, &vertical_raw_data->Z, 0.01f);
		}

}
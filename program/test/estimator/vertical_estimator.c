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

void vertical_sense(void){


}
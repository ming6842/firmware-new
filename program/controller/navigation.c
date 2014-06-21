//navigation.c
#include "navigation.h"

// NED -> XYZ so, N~x, E~y
// lat=N/S -> x, lon=E/W -> y


void PID_Nav(nav_pid_t *PID_control,attitude_t *attitude,UBXvelned_t *UBXvelned, UBXposLLH_t *UBXposLLH){

	float S_heading= arm_sin_f32(attitude->yaw * (0.01745329252392f));
	float C_heading= arm_cos_f32(attitude->yaw * (0.01745329252392f));


	if( PID_control -> controller_status == CONTROLLER_ENABLE){

		(PID_control -> error.x) = (float)((PID_control -> setpoint.x) -(UBXposLLH->lat));
		(PID_control -> error.y) = (float)((PID_control -> setpoint.y) -(UBXposLLH->lon));

		float P_lat = (PID_control -> error.x)*(PID_control -> kp);
		float P_lon = (PID_control -> error.y)*(PID_control -> kp);

		float D_N = -(float)(UBXvelned -> velN) * (PID_control -> kd);
		float D_E = -(float)(UBXvelned -> velE) * (PID_control -> kd);


		float P_roll_control = -P_lat*S_heading + P_lon*C_heading;
		float P_pitch_control = 0.0f -P_lat*C_heading - P_lon*S_heading;

		float D_roll_control = -D_N*S_heading + D_E*C_heading;
		float D_pitch_control = 0.0f -D_N*C_heading - D_E*S_heading;


		D_roll_control   = bound_float(D_roll_control,-40.0f,40.0f);
		D_pitch_control  = bound_float(D_pitch_control ,-40.0f,40.0f);

		P_roll_control   = bound_float(P_roll_control,-40.0f,40.0f);
		P_pitch_control  = bound_float(P_pitch_control,-40.0f,40.0f);

		(PID_control -> output_roll) = P_roll_control+D_roll_control;
		(PID_control -> output_pitch) = P_pitch_control+D_pitch_control;

		(PID_control -> output_roll) = bound_float(PID_control -> output_roll,-50.0f,50.0f);
		(PID_control -> output_pitch) = bound_float(PID_control -> output_pitch,-50.0f,50.0f);
	}else{

		PID_control -> setpoint.x = UBXposLLH->lat;
		PID_control -> setpoint.y = UBXposLLH->lon;

		PID_control -> integral.x = 0.0f;
		PID_control -> integral.y = 0.0f;
		PID_control -> output_roll =0.0f;
		PID_control -> output_pitch =0.0f;
	}

} 


/* Original code


void PID_control_P_Only(){ 
S_heading = sin(MagHDG*0.01745329251994329576923690768489);
C_heading = cos(MagHDG*0.01745329251994329576923690768489);
Nav_lat_error = Nav_lat_setpoint - UBXposLLH.lat;
Nav_lon_error = Nav_lon_setpoint - UBXposLLH.lon;

Nav_VN_error = 0-UBXvelned.velN;
Nav_VE_error = 0-UBXvelned.velE;

// direction is like ~ if error is possitive, it means that 'you are not north enough'. 
// so positive error will make it go more north
float P_lat = (float)Nav_lat_error*Nav_lat_Kp; 
float P_lon = (float)Nav_lon_error*Nav_lon_Kp; 

float D_N = (float)Nav_VN_error*Nav_lat_Kd; 
float D_E = (float)Nav_VE_error*Nav_lon_Kd; 


float P_roll_control = -P_lat*S_heading + P_lon*C_heading;
float P_pitch_control = 0.0 -P_lat*C_heading - P_lon*S_heading;

float D_roll_control = -D_N*S_heading + D_E*C_heading;
float D_pitch_control = 0.0 -D_N*C_heading - D_E*S_heading;

D_roll_control = Bound_float(D_roll_control,40.0,-40.0);
D_pitch_control  = Bound_float(D_pitch_control ,40.0,-40.0);

P_roll_control = Bound_float(P_roll_control,40.0,-40.0);
P_pitch_control = Bound_float(P_pitch_control,40.0,-40.0);

//Nav_roll_control= -(P_lat+D_N)*S_heading + (P_lon+D_E)*C_heading;
//Nav_pitch_control= 0.0 -(P_lat+D_N)*C_heading - (P_lon+D_E)*S_heading;
Nav_roll_control = P_roll_control+D_roll_control;
Nav_pitch_control = P_pitch_control+D_pitch_control;


Nav_roll_control = Bound_float(Nav_roll_control,50.0,-50.0);

Nav_pitch_control = Bound_float(Nav_pitch_control,50.0,-50.0);
}


*/
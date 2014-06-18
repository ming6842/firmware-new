//navigation.c

/*


void PID_Nav_P_Only(){ 
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
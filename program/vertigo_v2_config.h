//Vertigo v2.0 board setting
#define USE_FUTABA 
#define USE_IMU_MPU9250
#define USE_MAG_HMC5983
#define USE_ADS1246_MPX6115A
#define STM32F427X
/*choose the external device*/
#define USE_MAGNETOMETER 0
#define USE_BAROMETER 0
#define USE_GPS 0
/*Set magnetometer*/
#if USE_MAGNETOMETER

#endif
/*Set barometer*/
#if USE_BAROMETER

#endif

/*Set GPS*/
#if USE_GPS

#endif

// Controller define

#define CONTROL_f (4000.0f)
#define CONTROL_DT 1.0f/(4000.0f)
#define USE_MAGNETIC_HEADING
//#define USE_GYROSCOPE_HEADING


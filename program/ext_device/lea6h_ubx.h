#ifndef FILE_LEA6H_UBX_H
#define FILE_LEA6H_UBX_H

#include "led.h" 
#include <string.h>
#include "FreeRTOS.h"


#define GPS_BUFFER_SIZE 500


typedef struct UBXvelned_t{
		
	uint32_t itow;
	int32_t velN;
	int32_t velE;
	int32_t velD;
	uint32_t speed3D;
	uint32_t speed2D;
	int32_t hdg2D;
	uint32_t speedAccu;
	uint32_t hdgAccu;
	uint8_t updatedFlag;
}UBXvelned_t;

typedef struct UBXsol_t{
		
	uint32_t itow;
	int32_t ftow;
	int16_t week;
	uint8_t gpsFix;
	uint8_t statusFlag;
	int32_t ecefX;
	int32_t ecefY;
	int32_t ecefZ;
	uint32_t pAcc;
	int32_t ecefVX;
	int32_t ecefVY;
	int32_t ecefVZ;
	uint32_t vAcc;
	uint16_t pDOP;
	uint8_t numSV;
	uint8_t updatedFlag;

}UBXsol_t;

typedef struct UBXposLLH_t{
		
	uint32_t itow;
	int32_t lat;
	int32_t lon;
	int32_t hEllipsoid;
	int32_t hMSL;
	uint32_t hAcc;
	uint32_t zAcc;
	uint8_t updatedFlag;
}UBXposLLH_t;


void UART4_IRQHandler(void);
void lea6h_set_USART_IT(void);
void lea6h_ubx_get_updated_data(UBXvelned_t *,UBXsol_t *, UBXposLLH_t *);
void gps_receive_task(void);
void ubx_package_parser(void);
char uart4_read(void);
#endif
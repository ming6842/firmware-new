#include "lea6h_ubx.h"

uint8_t ubxBuffer[GPS_BUFFER_SIZE];

uint8_t *p;
uint16_t dataIndex = 0;
uint8_t capturingFlag = 0;
uint8_t bufferFullFlag = 0;
uint8_t bufferGetStop = 0;
uint8_t gotStartBitFlag=0;
uint8_t msgClass=0x00;
uint8_t msgID=0x00;
uint8_t chksumOKFlag=0;

uint8_t CK_A=0;
uint8_t CK_B=0;
uint16_t msgLength=0;

UBXvelned_t UBXvelned;
UBXsol_t UBXsol;
UBXposLLH_t UBXposLLH;


void lea6h_set_USART_IT(void){


	USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct = {
		.NVIC_IRQChannel = UART4_IRQn,
		.NVIC_IRQChannelPreemptionPriority =  1,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE
	};
	NVIC_Init(&NVIC_InitStruct);
}

void UART4_IRQHandler(void)
{

	uint16_t i=0;
	uint8_t c;

	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) {
		c = USART_ReceiveData(UART4);
		if((c == 0xB5)&&(capturingFlag==0)&&(bufferFullFlag==0)){
		capturingFlag=1;
		dataIndex=0;	

		}else if((capturingFlag==1)&&(ubxBuffer[0]==0xB5)&&(dataIndex==0)){
			if(c==0x62){
			dataIndex=1;
			gotStartBitFlag=1;
			//LED_ON(LED2);
			
			}else{
					capturingFlag=0;  /// Discard Data
				}
		}else if((dataIndex >0)&&(gotStartBitFlag)&&(capturingFlag==1)){    // Data capture is in process
			
			if(dataIndex == 1){
				dataIndex = 2;
				if(c==0x01){
					msgClass=1;
				}else{ // Got False Msg
					msgClass=0;
					capturingFlag=0;
					gotStartBitFlag=0;
					msgID=0;
				}

			}else if(dataIndex == 2){
				dataIndex = 3;
				
				if(c==0x06){
					msgID=0x06;
					msgLength=52;
//					putc(0x99,HARDU);
				}
				else if(c==0x12){
					msgID=0x12;
					msgLength=36;
//					putc(0x55,HARDU);
				}
				else if(c==0x02){
					msgID=0x02;
					msgLength=28;
				}
				else { // Got False Msg
					msgClass=0;
					capturingFlag=0;
					gotStartBitFlag=0;
					msgID=0;
//					putc(c,HARDU);
					dataIndex=0;
				}

			}else if((dataIndex >2)&&(dataIndex<msgLength+6)){
			dataIndex++;
				
			}else { // End of data length
			dataIndex++;

		ubxBuffer[dataIndex] = c;
				//LED_OFF(LED2);
			bufferGetStop = 1;
				///// CheckSum /////	
			CK_A=0;
			CK_B=0;
			for (i=0+2;i<msgLength+4+2;i++){
		
			CK_A=CK_A+ubxBuffer[i];
			CK_B=CK_B+CK_A;
		
			}
			if(((uint8_t)CK_A == ubxBuffer[msgLength+8-2])&&((uint8_t)CK_B == ubxBuffer[msgLength+8-1])){
				chksumOKFlag=1;

				if(msgID==0x12){
					p=&UBXvelned.itow;
					p[0]=ubxBuffer[6];
					p[1]=ubxBuffer[7];
					p[2]=ubxBuffer[8];
					p[3]=ubxBuffer[9];
			
					p=&UBXvelned.velN;
					p[0]=ubxBuffer[10];
					p[1]=ubxBuffer[11];
					p[2]=ubxBuffer[12];
					p[3]=ubxBuffer[13];
			
					p=&UBXvelned.velE;
					p[0]=ubxBuffer[14];
					p[1]=ubxBuffer[15];
					p[2]=ubxBuffer[16];
					p[3]=ubxBuffer[17];

					p=&UBXvelned.velD;
					p[0]=ubxBuffer[18];
					p[1]=ubxBuffer[19];
					p[2]=ubxBuffer[20];
					p[3]=ubxBuffer[21];

					p=&UBXvelned.speed3D;
					p[0]=ubxBuffer[22];
					p[1]=ubxBuffer[23];
					p[2]=ubxBuffer[24];
					p[3]=ubxBuffer[25];

					p=&UBXvelned.speed2D;
					p[0]=ubxBuffer[26];
					p[1]=ubxBuffer[27];
					p[2]=ubxBuffer[28];
					p[3]=ubxBuffer[29];

					p=&UBXvelned.hdg2D;
					p[0]=ubxBuffer[30];
					p[1]=ubxBuffer[31];
					p[2]=ubxBuffer[32];
					p[3]=ubxBuffer[33];

					p=&UBXvelned.speedAccu;
					p[0]=ubxBuffer[34];
					p[1]=ubxBuffer[35];
					p[2]=ubxBuffer[36];
					p[3]=ubxBuffer[37];

					p=&UBXvelned.hdgAccu;
					p[0]=ubxBuffer[38];
					p[1]=ubxBuffer[39];
					p[2]=ubxBuffer[40];
					p[3]=ubxBuffer[41];

					UBXvelned.updatedFlag = 1;
				}else if(msgID==0x06){

					p=&UBXsol.itow;
					p[0]=ubxBuffer[6];
					p[1]=ubxBuffer[7];
					p[2]=ubxBuffer[8];
					p[3]=ubxBuffer[9];

					p=&UBXsol.ftow;
					p[0]=ubxBuffer[10];
					p[1]=ubxBuffer[11];
					p[2]=ubxBuffer[12];
					p[3]=ubxBuffer[13];

					p=&UBXsol.week;
					p[0]=ubxBuffer[14];
					p[1]=ubxBuffer[15];

					p=&UBXsol.gpsFix;
					p[0]=ubxBuffer[16];
					
					p=&UBXsol.statusFlag;
					p[0]=ubxBuffer[17];

					p=&UBXsol.ecefX;
					p[0]=ubxBuffer[18];
					p[1]=ubxBuffer[19];
					p[2]=ubxBuffer[20];
					p[3]=ubxBuffer[21];

					p=&UBXsol.ecefY;
					p[0]=ubxBuffer[22];
					p[1]=ubxBuffer[23];
					p[2]=ubxBuffer[24];
					p[3]=ubxBuffer[25];

					p=&UBXsol.ecefZ;
					p[0]=ubxBuffer[26];
					p[1]=ubxBuffer[27];
					p[2]=ubxBuffer[28];
					p[3]=ubxBuffer[29];

					p=&UBXsol.pAcc;
					p[0]=ubxBuffer[30];
					p[1]=ubxBuffer[31];
					p[2]=ubxBuffer[32];
					p[3]=ubxBuffer[33];

					p=&UBXsol.ecefVX;
					p[0]=ubxBuffer[34];
					p[1]=ubxBuffer[35];
					p[2]=ubxBuffer[36];
					p[3]=ubxBuffer[37];

					p=&UBXsol.ecefVY;
					p[0]=ubxBuffer[38];
					p[1]=ubxBuffer[39];
					p[2]=ubxBuffer[40];
					p[3]=ubxBuffer[41];

					p=&UBXsol.ecefVZ;
					p[0]=ubxBuffer[42];
					p[1]=ubxBuffer[43];
					p[2]=ubxBuffer[44];
					p[3]=ubxBuffer[45];

					p=&UBXsol.vAcc;
					p[0]=ubxBuffer[46];
					p[1]=ubxBuffer[47];
					p[2]=ubxBuffer[48];
					p[3]=ubxBuffer[49];

					p=&UBXsol.pDOP;
					p[0]=ubxBuffer[50];
					p[1]=ubxBuffer[51];

					p=&UBXsol.numSV;
					p[0]=ubxBuffer[53];


					UBXsol.updatedFlag = 1;
				}else if(msgID==0x02){

					p=&UBXposLLH.itow;
					p[0]=ubxBuffer[6];
					p[1]=ubxBuffer[7];
					p[2]=ubxBuffer[8];
					p[3]=ubxBuffer[9];

					p=&UBXposLLH.lon;
					p[0]=ubxBuffer[10];
					p[1]=ubxBuffer[11];
					p[2]=ubxBuffer[12];
					p[3]=ubxBuffer[13];

					p=&UBXposLLH.lat;
					p[0]=ubxBuffer[14];
					p[1]=ubxBuffer[15];
					p[2]=ubxBuffer[16];
					p[3]=ubxBuffer[17];

					p=&UBXposLLH.hEllipsoid;
					p[0]=ubxBuffer[18];
					p[1]=ubxBuffer[19];
					p[2]=ubxBuffer[20];
					p[3]=ubxBuffer[21];

					p=&UBXposLLH.hMSL;
					p[0]=ubxBuffer[22];
					p[1]=ubxBuffer[23];
					p[2]=ubxBuffer[24];
					p[3]=ubxBuffer[25];

					p=&UBXposLLH.hAcc;
					p[0]=ubxBuffer[26];
					p[1]=ubxBuffer[27];
					p[2]=ubxBuffer[28];
					p[3]=ubxBuffer[29];

					p=&UBXposLLH.zAcc;
					p[0]=ubxBuffer[30];
					p[1]=ubxBuffer[31];
					p[2]=ubxBuffer[32];
					p[3]=ubxBuffer[33];


				LED_TOGGLE(LED2);
					UBXposLLH.updatedFlag = 1;
				if(UBXsol.pAcc>(int32_t)700){
					LED_TOGGLE(LED1);
				}else{
					LED_ON(LED1);
				}
				}
				
			}	else{


			}
			

					msgClass=0;
					gotStartBitFlag=0;
					msgID=0;
					bufferFullFlag=0;
					capturingFlag=0;
					dataIndex=0;
			}
				
		}
		




				// Too big data//
				if(dataIndex == GPS_BUFFER_SIZE-1){ 
				bufferFullFlag=0;
				capturingFlag=0;
				dataIndex = 0;
				//LED_OFF(LED2);
//				putc(0xFA,HARDU);
				}
		ubxBuffer[dataIndex] = c;




		LED_TOGGLE(LED1);
	} 

}

void lea6h_ubx_get_updated_data(UBXvelned_t *velned,UBXsol_t *sol, UBXposLLH_t *posLLH){

	if(UBXposLLH.updatedFlag==1){

		posLLH -> itow 			= UBXposLLH.itow ;
		posLLH -> lat 			= UBXposLLH.lat ;
		posLLH -> lon 			= UBXposLLH.lon ;
		posLLH -> hEllipsoid 	= UBXposLLH.hEllipsoid ;
		posLLH -> hMSL 			= UBXposLLH.hMSL ;
		posLLH -> hAcc 			= UBXposLLH.hAcc ;
		posLLH -> zAcc 			= UBXposLLH.zAcc ;
		posLLH -> updatedFlag 	= UBXposLLH.updatedFlag ;

		UBXposLLH.updatedFlag=0;
	}

	if(UBXsol.updatedFlag==1){

		sol -> itow			= UBXsol.itow;
		sol -> ftow			= UBXsol.ftow;
		sol -> week			= UBXsol.week;
		sol -> gpsFix		= UBXsol.gpsFix;
		sol -> statusFlag	= UBXsol.statusFlag;
		sol -> ecefX		= UBXsol.ecefX;
		sol -> ecefY		= UBXsol.ecefY;
		sol -> ecefZ		= UBXsol.ecefZ;
		sol -> pAcc			= UBXsol.pAcc;
		sol -> ecefVX		= UBXsol.ecefVX;
		sol -> ecefVY		= UBXsol.ecefVY;
		sol -> ecefVZ		= UBXsol.ecefVZ;
		sol -> vAcc			= UBXsol.vAcc;
		sol -> pDOP			= UBXsol.pDOP;
		sol -> numSV		= UBXsol.numSV;
		sol -> updatedFlag	= UBXsol.updatedFlag;
	
		UBXsol.updatedFlag=0;
	}


	if(UBXvelned.updatedFlag==1){

		velned -> itow			= UBXvelned.itow;
		velned -> velN 			= UBXvelned.velN;
		velned -> velE 			= UBXvelned.velE;
		velned -> velD 			= UBXvelned.velD;
		velned -> speed3D		= UBXvelned.speed3D;
		velned -> speed2D		= UBXvelned.speed2D;
		velned -> hdg2D			= UBXvelned.hdg2D;
		velned -> speedAccu		= UBXvelned.speedAccu;
		velned -> hdgAccu		= UBXvelned.hdgAccu;
		velned -> updatedFlag	= UBXvelned.updatedFlag;

		UBXvelned.updatedFlag=0;
	}

}
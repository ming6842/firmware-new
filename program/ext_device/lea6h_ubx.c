#include "lea6h_ubx.h"
#include "usart.h"
#include "queue.h"
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
extern xQueueHandle gps_serial_queue;

void lea6h_set_USART_IT(void){


	USART_ITConfig(UART4, USART_IT_TXE, DISABLE);
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);

	NVIC_InitTypeDef NVIC_InitStruct = {
		.NVIC_IRQChannel = UART4_IRQn,
		.NVIC_IRQChannelPreemptionPriority =  configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY+1,
		.NVIC_IRQChannelSubPriority = 0,
		.NVIC_IRQChannelCmd = ENABLE
	};
	NVIC_Init(&NVIC_InitStruct);
}
char uart4_read(void)
{
	serial_msg msg;

	while (!xQueueReceive(gps_serial_queue, &msg, portMAX_DELAY));

	return msg.ch;
}

void UART4_IRQHandler(void)
{

	long lHigherPriorityTaskWoken = pdFALSE;

	serial_msg rx_msg;


	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET) {
		rx_msg.ch = USART_ReceiveData(UART4);

		if (!xQueueSendToBackFromISR(gps_serial_queue, &rx_msg, &lHigherPriorityTaskWoken))
			portEND_SWITCHING_ISR(lHigherPriorityTaskWoken);

	}

	portEND_SWITCHING_ISR(lHigherPriorityTaskWoken);

}
void ubx_package_parser()
{
	uint16_t i=0;
	uint8_t c;
	c = uart4_read();
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

				memcpy(&UBXvelned.itow,&ubxBuffer[6],sizeof(uint32_t));
				memcpy(&UBXvelned.velN,&ubxBuffer[10],sizeof(uint32_t));
				memcpy(&UBXvelned.velE,&ubxBuffer[14],sizeof(uint32_t));
				memcpy(&UBXvelned.velD,&ubxBuffer[18],sizeof(uint32_t));
				memcpy(&UBXvelned.speed3D,&ubxBuffer[22],sizeof(uint32_t));
				memcpy(&UBXvelned.speed2D,&ubxBuffer[26],sizeof(uint32_t));
				memcpy(&UBXvelned.hdg2D,&ubxBuffer[30],sizeof(uint32_t));
				memcpy(&UBXvelned.speedAccu,&ubxBuffer[34],sizeof(uint32_t));
				memcpy(&UBXvelned.hdgAccu,&ubxBuffer[38],sizeof(uint32_t));

				UBXvelned.updatedFlag = 1;
			}else if(msgID==0x06){

				memcpy(&UBXsol.itow,&ubxBuffer[6],sizeof(uint32_t));
				memcpy(&UBXsol.ftow,&ubxBuffer[10],sizeof(uint32_t));
				memcpy(&UBXsol.week,&ubxBuffer[14],sizeof(uint16_t));
				memcpy(&UBXsol.gpsFix,&ubxBuffer[16],sizeof(uint8_t));
				memcpy(&UBXsol.statusFlag,&ubxBuffer[17],sizeof(uint8_t));
				memcpy(&UBXsol.ecefX,&ubxBuffer[18],sizeof(uint32_t));
				memcpy(&UBXsol.ecefY,&ubxBuffer[22],sizeof(uint32_t));
				memcpy(&UBXsol.ecefZ,&ubxBuffer[26],sizeof(uint32_t));
				memcpy(&UBXsol.pAcc,&ubxBuffer[30],sizeof(uint32_t));
				memcpy(&UBXsol.ecefVX,&ubxBuffer[34],sizeof(uint32_t));
				memcpy(&UBXsol.ecefVY,&ubxBuffer[38],sizeof(uint32_t));
				memcpy(&UBXsol.ecefVZ,&ubxBuffer[42],sizeof(uint32_t));
				memcpy(&UBXsol.vAcc,&ubxBuffer[46],sizeof(uint32_t));
				memcpy(&UBXsol.pDOP,&ubxBuffer[50],sizeof(uint16_t));
				memcpy(&UBXsol.numSV,&ubxBuffer[53],sizeof(uint8_t));


				UBXsol.updatedFlag = 1;
			}else if(msgID==0x02){



				memcpy(&UBXposLLH.itow,&ubxBuffer[6],sizeof(uint32_t));
				memcpy(&UBXposLLH.lon,&ubxBuffer[10],sizeof(uint32_t));
				memcpy(&UBXposLLH.lat,&ubxBuffer[14],sizeof(uint32_t));
				memcpy(&UBXposLLH.hEllipsoid,&ubxBuffer[18],sizeof(uint32_t));
				memcpy(&UBXposLLH.hMSL,&ubxBuffer[22],sizeof(uint32_t));
				memcpy(&UBXposLLH.hAcc,&ubxBuffer[26],sizeof(uint32_t));
				memcpy(&UBXposLLH.zAcc,&ubxBuffer[30],sizeof(uint32_t));


				UBXposLLH.updatedFlag = 1;
			if((UBXsol.pAcc>(int32_t)700) || (UBXsol.numSV < (uint8_t)7)){
				LED_TOGGLE(LED2);
			}else{
				LED_ON(LED2);
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
}

void lea6h_ubx_get_updated_data(UBXvelned_t *velned,UBXsol_t *sol, UBXposLLH_t *posLLH){

	if(UBXposLLH.updatedFlag==1){

		memcpy(posLLH,&UBXposLLH,sizeof(UBXposLLH_t));

		UBXposLLH.updatedFlag=0;
	}

	if(UBXsol.updatedFlag==1){

		memcpy(sol,&UBXsol,sizeof(UBXsol_t));

		UBXsol.updatedFlag=0;
	}


	if(UBXvelned.updatedFlag==1){

		memcpy(velned,&UBXvelned,sizeof(UBXvelned_t));

		UBXvelned.updatedFlag=0;
	}

}
void gps_receive_task()
{
	while(1) {

		ubx_package_parser();

	}
}
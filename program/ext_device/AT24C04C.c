#include <string.h>
#include "stm32f4xx_conf.h"
#include "AT24C04C.h"

#define EEPROM_DEVICE_BASE_ADDRESS 0xA8
#define EEPROM_WORD_BASE_ADDRESS 0x00

#define EEPROM_PAGE_SIZE 16

eeprom_t eeprom = {
	.read = eeprom_read,
	.write = eeprom_write
};

static void eeprom_page_write(uint8_t *data, uint8_t device_address, uint8_t word_address, 
	int data_count)
{
	/* Send the I2C start condition */
	I2C_GenerateSTART(I2C1, ENABLE);
 	 
	/* Test on I2C EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)); 
  
	/* Send EEPROM address for write */
	I2C_Send7bitAddress(I2C1, device_address, I2C_Direction_Transmitter);
  
	/* Test on I2C EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));  

	/* Send the EEPROM word address */    
	I2C_SendData(I2C1, word_address);  

	/* Test on I2C EV8 and clear it */
	while(! I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	while(data_count--)  
	{
		/* Send the current byte */
		I2C_SendData(I2C1, *data); 

		/* Point to the next byte to be written */
		data++; 
  
		/* Test on I2C EV8 and clear it */
		while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	}

	/* Send the I2C stop condition */
	I2C_GenerateSTOP(I2C1, ENABLE);
}

void eeprom_write(uint8_t *buffer, int count)
{
	int data_left = count;

	uint8_t device_address = EEPROM_DEVICE_BASE_ADDRESS, word_address = 0;

	/* Calculate the page count to store the data */
	int page_usage = count / EEPROM_PAGE_SIZE;
	page_usage += (count % EEPROM_PAGE_SIZE) > 0 ? 1 : 0; //Need to carry or not

	/* Page writing operation */
	int used_page_count;
	for(used_page_count = 0; used_page_count < page_usage; used_page_count++) {
		uint8_t data[EEPROM_PAGE_SIZE] = {0};
		/* Calculate how many space can use in current EEPROM page */
		int page_left_space = EEPROM_PAGE_SIZE - eeprom._write.page_offset;

		/* Data copy */
		int i;
		for(i = 0; i < page_left_space; i++);
			data[i] = buffer[i];

		/* Calculate the device adrress and the word address */
		//Set device address bit 2 and 3
		device_address |= (eeprom._write.page >> 4 << 1);
		//Set word address bit 5 to 8
		word_address |= eeprom._write.page << 4;
		//Set word address bit 1 to 4;
		word_address |= eeprom._write.page_offset;	

		/* Write the data in the page */
		if(data_left >= page_left_space) {
			/* The page is going to be full */
			eeprom_page_write(data, device_address, word_address, page_left_space);
			data_left -= page_left_space;

			/* Point the EEPROM to next page */
			eeprom._write.page++;
			eeprom._write.page_offset = 0;			
		} else {
			/* There will be some empty space in this page after the write 
			   operation */
			eeprom_page_write(data, device_address, word_address, data_left);
			/* Increase the EEPROM page offset */
			eeprom._write.page_offset += data_left;
		}
	}
}

void eeprom_sequential_read(uint8_t *buffer, uint8_t device_address, uint8_t word_address,
	int buffer_count)
{  
	while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
    
	/* Send the I2C start condition */
	I2C_GenerateSTART(I2C1, ENABLE);
  
	/* Test on I2C EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

	/* Send the device address */
	I2C_Send7bitAddress(I2C1, device_address, I2C_Direction_Transmitter);

	/* Test on I2C EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
  
	/* Clear the I2C EV6 by setting again the PE bit */
	I2C_Cmd(I2C1, ENABLE);

	/* Send the word_address */
	I2C_SendData(I2C1, word_address);  

	/* Test on I2C EV8 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
	/* Send the start condition a second time */  
	I2C_GenerateSTART(I2C1, ENABLE);
  
	/* Test on I2C EV5 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
  
	/* Send the device address */
	I2C_Send7bitAddress(I2C1, device_address, I2C_Direction_Receiver);
  
	/* Test on I2C EV6 and clear it */
	while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));
  
	while(buffer_count)  
	{
		if(buffer_count == 1) {
			/* Disable Acknowledgement */
			I2C_AcknowledgeConfig(I2C1, DISABLE);
      
			/* Send STOP Condition */
			I2C_GenerateSTOP(I2C1, ENABLE);
		}

		/* Test on EV7 and clear it */
		if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) {      
			/* Read a byte from the EEPROM */
			*buffer = I2C_ReceiveData(I2C1);

			/* Point to the next location where the byte read will be saved */
			buffer++; 

			/* Decrement the read bytes counter */
			buffer_count--;        
		}   
	}

	/* Enable Acknowledgement to be ready for another reception */
	I2C_AcknowledgeConfig(I2C1, ENABLE);
}

void eeprom_read(uint8_t *data, int count)
{
	int data_count = 0;
	int data_left = count;

	uint8_t device_address = EEPROM_DEVICE_BASE_ADDRESS, word_address = 0;

	/* Calculate the page count to store the data */
	int page_usage = count / EEPROM_PAGE_SIZE;
	page_usage += (count % EEPROM_PAGE_SIZE) > 0 ? 1 : 0; //Need to carry or not

	/* Page writing operation */
	int used_page_count;
	for(used_page_count = 0; used_page_count < page_usage; used_page_count++) {
		uint8_t buffer[EEPROM_PAGE_SIZE] = {0};
		/* Calculate how many space can read in current EEPROM page */
		int page_left_space = EEPROM_PAGE_SIZE - eeprom._read.page_offset;

		/* Calculate the device adrress and the word address */
		//Set device address bit 2 and 3
		device_address |= (eeprom._read.page >> 4 << 1);
		//Set word address bit 5 to 8
		word_address |= eeprom._read.page << 4;
		//Set word address bit 1 to 4;
		word_address |= eeprom._read.page_offset;	

		/* Read the data from the page */
		if(data_left >= page_left_space) {
			/* The page is going to be full */
			eeprom_sequential_read(buffer, device_address, word_address, page_left_space);
			data_left -= page_left_space;

			/* Data copy */
			memcpy(data + data_count, buffer, page_left_space);	
			data_count += page_left_space;

			/* Point the EEPROM to next page */
			eeprom._read.page++;
			eeprom._read.page_offset = 0;			
		} else {
			/* There will be some empty space in this page after the read
			   operation */
			eeprom_sequential_read(buffer, device_address, word_address, data_left);

			/* Data copy */
			memcpy(data + data_count, buffer, data_left);
			data_count += data_left;

			/* Increase the EEPROM page offset */
			eeprom._read.page_offset += data_left;
		}
	}

}

void I2C_EE_WaitEepromStandbyState(void)      
{
  vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(I2C1, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, EEPROM_DEVICE_BASE_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(I2C1, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C1, I2C_FLAG_AF);
    /* STOP condition */    
    I2C_GenerateSTOP(I2C1, ENABLE); 
}

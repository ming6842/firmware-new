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

void eeprom_write(uint8_t *data, uint16_t eeprom_address,  int count)
{
	int data_left = count;
	uint8_t device_address = EEPROM_DEVICE_BASE_ADDRESS, word_address = 0x00;

	/* Calculate the page count to store the data */
	int page_usage = count / EEPROM_PAGE_SIZE;
	page_usage += (count % EEPROM_PAGE_SIZE) > 0 ? 1 : 0; //Need to carry or not

	/* Calulate the start page and page byte offset */
	uint8_t current_write_page = eeprom_address / EEPROM_PAGE_SIZE;
	//Get the byte offset of current write page
	uint8_t current_page_write_byte = count % EEPROM_PAGE_SIZE;

	/* Page write operation */
	int used_page_count;
	for(used_page_count = 0; used_page_count < page_usage; used_page_count++) {
		/* Current page information */
		uint8_t page_buffer[EEPROM_PAGE_SIZE] = {0};
		int page_left_space = EEPROM_PAGE_SIZE - current_page_write_byte;

		/* Calculate the device adrress and the word address (Only high 4 bit) */
		//Set device address bit 2 and 3
		device_address |= (current_write_page >> 4 << 1);
		//Set word address bit 5 to 8
		word_address |= current_write_page << 4;
		//Set word address bit 1 to 4
		word_address |= current_page_write_byte;

		/* Write the data in current page */
		if(data_left >= page_left_space) {
			/* Fill the full page by writing data */
			memcpy(page_buffer, data + (count - data_left),
				EEPROM_PAGE_SIZE - current_page_write_byte);
			eeprom_page_write(page_buffer, device_address, word_address,
				EEPROM_PAGE_SIZE - current_page_write_byte);

			data_left -= EEPROM_PAGE_SIZE - current_page_write_byte;

			/* Point to next page */
			current_write_page++;
			current_page_write_byte = 0;
		} else {
			/* Write the data into current page */
			memcpy(page_buffer, data + (count - data_left), data_left);
			eeprom_page_write(page_buffer, device_address, word_address,
				data_left);

			/* Increase the EEPROM page offset */
			current_page_write_byte += data_left;
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

void eeprom_read(uint8_t *data, uint16_t eeprom_address,int count)
{
	int data_left = count;
	uint8_t device_address = EEPROM_DEVICE_BASE_ADDRESS, word_address = 0x00;

	/* Calculate the page count to store the data */
	int page_usage = count / EEPROM_PAGE_SIZE;
	page_usage += (count % EEPROM_PAGE_SIZE) > 0 ? 1 : 0; //Need to carry or not

	/* Calulate the start page and page byte offset */
	uint8_t current_read_page = eeprom_address / EEPROM_PAGE_SIZE;
	//Get the byte offset of current read page
	uint8_t current_page_read_byte = count % EEPROM_PAGE_SIZE;

	/* Page read operation */
	int used_page_count;
	for(used_page_count = 0; used_page_count < page_usage; used_page_count++) {
		uint8_t buffer[EEPROM_PAGE_SIZE] = {0};
		/* Calculate how many space can read in current EEPROM page */
		int page_left_space = EEPROM_PAGE_SIZE - current_page_read_byte;

		/* Calculate the device adrress and the word address */
		//Set device address bit 2 and 3
		device_address |= (current_read_page >> 4 << 1);
		//Set word address bit 5 to 8
		word_address |= current_read_page << 4;
		//Set word address bit 1 to 4;
		word_address |= current_page_read_byte;

		/* Read the data from the page */
		if(data_left >= page_left_space) {
			/* The page is going to be full */
			eeprom_sequential_read(buffer, device_address, word_address, page_left_space);

			/* Return the data */
			memcpy(data + (count - data_left), buffer, page_left_space);
			data_left -= page_left_space;

			/* Point the current EEPROM page to next page */
			current_read_page++;
			current_page_read_byte = 0;			
		} else {
			/* There will be some empty space in this page after the read
			   operation */
			eeprom_sequential_read(buffer, device_address, word_address, data_left);

			/* Return the data */
			memcpy(data + (count - data_left), buffer, data_left);

			/* Increase the current EEPROM page offset */
			current_page_read_byte += data_left;
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

#ifndef FILE_AT24C04C_H
#define FILE_AT24C04C_H

/* EEPROM */
typedef struct {
	void (*write)(uint8_t *data, uint16_t eeprom_address, int count);
	void (*read)(uint8_t *data, uint16_t eeprom_address, int count);
} eeprom_t;

extern eeprom_t eeprom;

enum {
	EEPROM_SUCCESS,
	EEPROM_INVALID_ADDRESS,
	EEPROM_BUFFER_OVERFLOW
} EEPROM_Exit_Status;

#endif

#ifndef FILE_AT24C04C_H
#define FILE_AT24C04C_H

/* EEPROM */
typedef struct {
	int (*write)(uint8_t *data, uint16_t eeprom_address, uint16_t count);
	int (*read)(uint8_t *data, uint16_t eeprom_address, uint16_t count);
} eeprom_t;

extern eeprom_t eeprom;

enum {
	EEPROM_SUCCESS,
	EEPROM_INVALID_ADDRESS,
	EEPROM_BUFFER_OVERFLOW
} EEPROM_Exit_Status;

#endif

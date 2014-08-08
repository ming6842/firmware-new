#ifndef FILE_AT24C04C_H
#define FILE_AT24C04C_H

void eeprom_read(uint8_t *data, int count);
void eeprom_write(uint8_t *data, int count);

/* EEPROM */
typedef struct {
	struct {
		uint8_t page;
		uint8_t page_offset; 
	} _read;

	struct {
		uint8_t page;
		uint8_t page_offset;
	} _write;

	void (*write)(uint8_t *buffer, int count);
	void (*read)(uint8_t *data, int count);
} eeprom_t;

#endif

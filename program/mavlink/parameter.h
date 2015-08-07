#ifndef __PARAMETER_H
#define __PARAMETER_H

enum {
	PARAMETER_IDLE,
	/* Parameter read protocol */
	PARAMETER_READ_REQUEST,
	/* Parameter read single protocol */
	PARAMETER_READ_SINGLE_REQUEST,
	/* Parameter write protocol */
	PARAMETER_SET_REQUEST
} ParameterState;

void parameter_read_value(void);
void parameter_read_single_value(void);
void parameter_write_value(void);

#endif

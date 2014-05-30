#ifndef __GLOBAL_H
#define __GLOBAL_H
enum {
	FIRST_VALUE,
	VAR_CNT
};

typedef struct  {
	char *name;
	volatile float value;
} global_t;

global_t variable[VAR_CNT];


#endif
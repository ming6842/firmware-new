#include <unistd.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "std.h"
#include "usart.h"
#include "io.h"
int printf_base(const char *format, ...)
{
	char str[128];
	va_list para;
	va_start(para, format);
	int curr_pos = 0;
	char ch[] = {'0', '\0'};
	char integer[11];
	str[0] = '\0';

	while (format[curr_pos] != '\0') {
		if (format[curr_pos] != '%') {
			ch[0] = format[curr_pos];
			strcat(str, ch);

		} else {
			switch (format[++curr_pos]) {
			case 's':
				strcat(str, va_arg(para, char *));
				break;

			case 'c':
				ch[0] = (char)va_arg(para, int);
				strcat(str, ch);
				break;

			case 'i':
			case 'f':
				strcat(str, ftoa(va_arg(para, double)));
				break;

			case 'd':
				strcat(str, itoa(va_arg(para, int), integer));
				break;

			case 'u':
				strcat(str, itoa(va_arg(para, unsigned), integer));
				break;

			default:
				break;
			}
		}

		curr_pos++;
	}

	va_end(para);
	uart8_puts((uint8_t *)str);
	return 1;
}
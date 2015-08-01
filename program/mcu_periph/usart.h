#ifndef FILE_USART_H
#define FILE_USART_H
#include <stdint.h>
#include <stdbool.h>

typedef struct {
        char ch;
} serial_msg;

void usart_init(void);
void retarget_init(void);
int _write(int fd, char *ptr, int len);
int _read(int fd, char *ptr, int len);
void _ttywrch(int ch);
void usart2_dma_send(uint8_t *s);
void usart2_dma_init(void);

char usart3_read(void);
void usart3_send(char str);
bool is_usart3_rx_available(void);
void USART3_IRQHandler(void);
void uart8_puts(uint8_t *ptr);
#endif

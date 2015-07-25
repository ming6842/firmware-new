#ifndef FILE_USART_H
#define FILE_USART_H
#include <stdint.h>
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
void USART3_IRQHandler(void);
void uart8_puts(uint8_t *ptr);
void DMA1_Stream3_IRQHandler(void);
void usart3_dma_send(uint8_t *, uint16_t );
#endif

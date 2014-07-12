#ifndef FILE_USART_H
#define FILE_USART_H

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
void usart8_send_string(uint8_t *ptr);
#endif

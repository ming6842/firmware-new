#ifndef FILE_USART_H
#define FILE_USART_H
void usart_init(void);
void retarget_init(void);
int _write(int fd, char *ptr, int len);
int _read(int fd, char *ptr, int len);
void _ttywrch(int ch);
void usart2_dma_send(uint8_t *s);
void usart2_dma_init(void);
#endif

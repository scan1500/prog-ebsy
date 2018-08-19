#ifndef UART0
#define UART0
#include "rbuf.h"

int uart0_init(void);
int uart0_open(void);
int uart0_close(void);
int uart0_read(uint8_t*,int);
int uart0_write(uint8_t*,int);
int uart0_ioctl(uint32_t,uint32_t);
void UART0_IRQHandler(void);
extern ringBuf uart0_rx, uart0_tx;

#endif

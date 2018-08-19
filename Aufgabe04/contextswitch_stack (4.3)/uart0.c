#include "ebsy.h"
#include <LPC177x_8x.h>
#include "uart0.h"

ringBuf	uart0_rx, uart0_tx;
ringBuf *my_uart0_rx = &uart0_rx;
ringBuf *my_uart0_tx = &uart0_tx;

int uart0_init(void) {
	LPC_IOCON->P0_2 = 1; // -> U0_TXD
	LPC_IOCON->P0_3 = 1; // -> U0_RXD
	/* DLAB-configuration to set up for correct baud rate. */
	/* baudrate = PCLK / (16 * (256 * DLM + DLL)) */
	/* baudrate here: 115200 */
	LPC_UART0->LCR = 0x80; // enable DLAB
	LPC_UART0->DLM = 0;
	//LPC_UART0->DLL = 16;
	LPC_UART0->DLL = 195;
	LPC_UART0->LCR = 3; // 8 bits wordlength, 1 stop-bit, no parity
	LPC_UART0->FCR = 1; // enable FIFO
	LPC_UART0->IER = 3;
	NVIC->ISER[0] = (1<<5);
	return 0;
}

int uart0_open(void) {
	return 0;
}

int uart0_close(void) {
	return 0;
}

int uart0_read(uint8_t* buf, int nbytes) {
	int bytes_read;
	for(bytes_read = 0; bytes_read < nbytes; bytes_read++) {
			ringBuf_get(my_uart0_rx);
	}
	return bytes_read;
}

int uart0_write(uint8_t *buf, int nbytes) {
	int bytes_written = 0;
	uint8_t *c = buf;
	while(bytes_written < nbytes) {
		ringBuf_put(my_uart0_tx, *c);
		c++;
		bytes_written++;
	}
	return bytes_written;
}

int uart0_ioctl(uint32_t request, uint32_t requestparam) {
	return 0;
}


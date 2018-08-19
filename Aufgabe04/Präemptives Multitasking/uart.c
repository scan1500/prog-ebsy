#include <LPC177x_8x.h>
#include <stdint.h>

void uart_init()
{
		//1. Power und Takt für UART0 aktivieren: PCONP-Register
		LPC_SC->PCONP |= (1 << 3);  // Set Bit 03 for UART0 (PC)
		LPC_SC->PCONP |= (1 << 24);	// Set Bit 24 for UART2 (Keyboard)
	
		//2. Erzeugung des Taktes PCLK für UART0 aus der AHB Peripherie-Clock. Erzeugt
		//durch Teilen des Prozessortaktes, konfiguriert in PCLKSEL (z.B. durch 1):
		//LPC_SC->PCLKSEL |= (1 << 4);// Peripheral Clock already set in system_LPC
	
		
		//0x4000C00C
	
		//3. Setzen der Baudrate: In Register LCR Bit DLAB auf „1“ setzen
		LPC_UART0->LCR |= (1<<7);
	
		//4. DLL & DLM anhand der Baudrate berechnen
		LPC_UART0->DLM = 1;
		LPC_UART0->DLL = 56;
	
		// Fractional: 0
		LPC_UART0->FDR |= (1<<4); 
	
		// FIFO enable
		LPC_UART0->FCR |= (1<<0); 
		
		// set DLAB=0 -> disable access to div. Latch
		LPC_UART0->LCR &= ~(1<<7); 
		
		// 8 bits, 1 stop bit, no parity
		LPC_UART0->LCR |= 3;
		
		// Pin config as UART RxD
		*IOCON_0_2 |= 1; 
		
		// Pin config as UART TxD
		*IOCON_0_3 |= 1; 
		
		
		LPC_UART0->RBR;
		
		LPC_UART0->THR;
		
		
		LPC_UART0->IER |= 1;
	
	
	
}

void UART0_IRQHandler(void)
{
		
}

void UART2_IRQHandler(void)
{
		
}
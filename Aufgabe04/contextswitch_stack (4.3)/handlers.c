#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <LPC177x_8x.h>

#include "ctxsw.h"
#include "ebsy.h"
#include "uart0.h"
#include "sched.h"

/* landing here, means we fucked up badly */
void HardFault_Handler(void) {
	uint32_t counter = 0;
	while(1) {
		if (counter % 2)
		{
			LPC_GPIO0->SET = 0x100;
			for(uint32_t i = 0;i < 240000;i++)
			{
			}
		}
		else
		{
			LPC_GPIO0->CLR = 0x100;
			for(uint32_t i = 0;i < 240000;i++)
			{
			}
		}
		counter++;
	}
}

void MemManage_Handler(void) {
	uint32_t counter = 0;
	while(1) {
		if (counter % 2)
		{
			LPC_GPIO0->SET = 0x100;
			for(uint32_t i = 0;i < 240000;i++)
			{
			}
		}
		else
		{
			LPC_GPIO0->CLR = 0x100;
			for(uint32_t i = 0;i < 240000;i++)
			{
			}
		}
		counter++;
	}
}

/* called, when the SystemTick-Timer generates an interrupt
 * vector is defined in startup_LPC177x_8x.s
 * registers saved on stack (in occuring order):
 * R0-R3,R12,LR,PC,xPSR
 */
void SysTick_Handler(void) {
    get_next_pid(); // schedules the next process
    cur_sp = (uintptr_t)&procs[cur_pid-1].sp; // needed in ctxsw, to write back the modified stack pointer
    next_sp = (uintptr_t)&procs[next_pid-1].sp; // needed in ctxsw to get current stack pointer of next process
    SCB->ICSR |= (1 << 28); // trigger PendSV
}

void UART0_IRQHandler(void) {
	ringBuf *my_uart0_rx = &uart0_rx;
	ringBuf *my_uart0_tx = &uart0_tx;
	if(LPC_UART0->LSR & 1) {
        // receive
        char c = LPC_UART0->RBR;
        ringBuf_put(my_uart0_rx,c);
	} else if (LPC_UART0->LSR & (1<<6)) {
        // send
        char c  = ringBuf_get(my_uart0_tx);
				if(c > 0)
					LPC_UART0->THR = c;
	}
}

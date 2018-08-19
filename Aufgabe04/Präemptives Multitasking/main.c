#include <LPC177x_8x.h>
#include <stdint.h>

// Typedef. Prozess-Tabelle
typedef struct 
{
		uint32_t pid;
		uintptr_t sp;
		uint32_t Interval;
		uint32_t LastTick;
		void (*Func)(void);
} proc;

// Funktionen
void fork(void);

// Prozesse
void p0(void);
void p1(void);
void p2(void);
void p3(void);

// Handler
void PendSV_Handler(void);
void SysTick_Handler(void);
void HardFault_Handler(void);

// ASM-Funktionen
extern void firstProc(uintptr_t sp);
extern void ctxsw(uintptr_t old_stack, uintptr_t new_stack);

// Init. Prozess-Tabelle 
static proc procs[15] = 
{
		{0,0,1,0,p0}, // P0
		{1,0,1,0,p1}, // P1
		{2,0,1,0,p2}, // P2
		{3,0,1,0,p3}	// P3
};

// Stackpointer | Prozess-IDs | SysTick-Controlregister - für ASM-Routine (Globaler Zugriff)
uint32_t numProcs = 4;
uintptr_t	cur_sp, next_sp, SysTick_CTRL;
uint32_t cur_pid = 0, next_pid = 0, test_fork = 1;

// Stackinit. (max. 0-255 Prozesse)
uint32_t stack[15][32]; 

int main(void)
{  
		//GPIO Setup
    LPC_GPIO0->DIR = 0xFF00;
	
		//SysTick & PendSV Priority Level 
		SCB->SHP[11] = 0; 				
    SCB->SHP[10] = 0x3; 			

		//SysTick Setup
		SysTick->LOAD = 0x0FFFFF;
		SysTick_CTRL = (uintptr_t)&(SysTick->CTRL);
	  //SysTick->VAL = 0;	
	
		// Einstieg für ersten Prozess: P1
    stack[cur_pid][31] = (uintptr_t)procs[cur_pid].Func;            
    procs[cur_pid].sp = (uintptr_t)&(stack[cur_pid][31]);

    // Stack Setup für jeden Prozess
    for(int proc_count = 1; proc_count < numProcs; proc_count++) 
		{
        stack[proc_count][31] = (uint32_t)0x21000000;               // xPSR
        stack[proc_count][30] = (uintptr_t)procs[proc_count].Func;  // PC
        stack[proc_count][29] = (uintptr_t)procs[proc_count].Func;  // LR
        stack[proc_count][28] = 0xdeadbeef;                         // R12
        stack[proc_count][27] = 0;                                  // R3
        stack[proc_count][26] = 0;                                  // R2
        stack[proc_count][25] = 0;                                  // R1
        stack[proc_count][24] = 0;                                  // R0
        stack[proc_count][23] = proc_count;                                  // R11
        stack[proc_count][22] = proc_count;                                  // R10
        stack[proc_count][21] = proc_count;                                  // R9
        stack[proc_count][20] = proc_count;                                  // R8
        stack[proc_count][19] = proc_count;                                  // R7
        stack[proc_count][18] = proc_count;                                  // R6
        stack[proc_count][17] = proc_count;                                  // R5
        stack[proc_count][16] = proc_count;                                  // R4
        procs[proc_count].sp = (uintptr_t)&(stack[proc_count][16]); // reserve space on stack
    }
		//Ersten Prozess mit Assembler Funktion "firstProc()" erzeugen
    firstProc(procs[cur_pid].sp); 
}

void p0(void)
{
		while(1)
			LPC_GPIO0->SET = 0x100;
}

void p1(void)
{
		/*
		if(test_fork)
		{
				fork();
				test_fork = 0;
		}
		*/
		while(1)
			LPC_GPIO0->SET = 0x200;
}

void p2(void)
{
		while(1)
			LPC_GPIO0->SET = 0x400;
}

void p3(void)
{
		while(1)
			LPC_GPIO0->CLR = 0x700;
}

void fork(void)
{
		for(uint8_t index = 16; index < 32; index++) 
			stack[numProcs][index] = stack[cur_pid][index];               

    procs[numProcs].Func = 0;
		procs[numProcs].Interval = 0;
		procs[numProcs].LastTick = 1;
		procs[numProcs].pid = numProcs;
		procs[numProcs].sp = (uintptr_t)&(stack[numProcs][16]); 
		
		numProcs++;
}

//Exception-Handler
void SysTick_Handler(void) 
{
		//Scheduling: Round-Robin 0->1->2->3->0
		if(++next_pid == numProcs)
			next_pid = 0;
		
		// Stackpointer werden in ctxsw geladen bzw neu gesetzt
    cur_sp = (uintptr_t)&procs[cur_pid].sp;
    next_sp = (uintptr_t)&procs[next_pid].sp;
		
		// PendSV triggern
    SCB->ICSR |= (1 << 28); 
}

void HardFault_Handler(void) 
{
		uint32_t counter = 0;
		while(1) 
		{
				if (counter % 2)
				{
						LPC_GPIO0->SET = 0x100;
						for(uint32_t i = 0; i < 240000; i++){}
				}	
				else
				{
						LPC_GPIO0->CLR = 0x100;
						for(uint32_t i = 0; i < 240000; i++){}
				}
				counter++;
		}
}

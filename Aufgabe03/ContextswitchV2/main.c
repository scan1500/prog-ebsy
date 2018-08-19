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

void p1(void);
void p2(void);
void yield(void);
void HardFault_Handler(void);
extern void firstProc(uintptr_t sp);
extern void ctxsw(uintptr_t old_stack, uintptr_t new_stack);

// Init. Prozess-Tabelle 
static proc procs[] = {{0,0,1,0,p1},{1,0,1,0,p2}};

// Anzahl Prozesse in Prozess-Tabelle 
const uint8_t numProcs = sizeof(procs) / sizeof(*procs);

uint32_t cur_pid = 0, next_pid = 1;
uint32_t stack[numProcs][32];
static uint32_t tick = 0;

int main(void)
{  
    LPC_GPIO0->DIR = 0xFF00;
	
    procs[0].sp = (uintptr_t)&(stack[0][31]); //SP von P0 oberste Adresse in Stack0 setzen
		procs[1].sp = (uintptr_t)&(stack[1][31]); //SP von P1 oberste Adresse in Stack1 setzen

		//Debug-Data, testing etc.
		stack[0][30] = 9; //R12
		stack[0][29] = 8; //R11
		stack[0][28] = 7; //R10
		stack[0][27] = 6; //R9
		stack[0][26] = 5; //R8
		stack[0][25] = 4; //R7
		stack[0][24] = 3; //R6
		stack[0][23] = 2; //R5
		stack[0][22] = 1; //R4
	
		stack[0][31] = (uintptr_t)p1; //Startfunktion P1
		stack[1][31] = (uintptr_t)p2; //Startfunktion P2
	
    firstProc(procs[0].sp - 9*4); //Ersten Prozess erzeugen -> Hierzu Assembler Funktion aufrufen
}
void p1(void)
{
	while(1)
	{
		LPC_GPIO0->SET = 0xFF00;
		yield();
	}
}

void p2(void)
{
	while(1)
	{
		LPC_GPIO0->CLR = 0xF000;
    yield();
	}
}

void yield()
{
		if (tick++ % 2 == 0)
		{
				cur_pid = 0;
        next_pid = 1;
		}
    else
		{
				cur_pid = 1;
        next_pid = 0;
		}
		//Funktionen werden immer neu aufgerufen, anstatt Prozess fortzusetzen...
		stack[next_pid][31] = (uint32_t)procs[next_pid].Func;
    ctxsw(procs[cur_pid].sp+4, procs[next_pid].sp - 9*4);
}

//Exception-Handler
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

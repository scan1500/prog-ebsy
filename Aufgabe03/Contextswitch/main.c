#include <LPC177x_8x.h>
#include <stdint.h>
#include "exceptions.h"
#include "ctxsw.h"

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
void get_next_pid(void);

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

		stack[0][30] = 9; //R12
		stack[0][29] = 8; //R11
		stack[0][28] = 7; //R10
		stack[0][27] = 6; //R9
		stack[0][26] = 5; //R8
		stack[0][25] = 4; //R7
		stack[0][24] = 3; //R6
		stack[0][23] = 2; //R5
		stack[0][22] = 1; //R4
	
    uint32_t pid = 0;
		stack[pid][31] = (uintptr_t)p1; //Startfunktion des Prozesses
    procs[pid].sp = (uintptr_t)&(stack[pid][31]) - 9*4; //Stackpointer auf unterste Adresse zeigen lassen

    firstProc(procs[pid].sp); //Ersten Prozess erzeugen -> Hierzu Assembler Funktion aufrufen
}
void p1(void)
{
		LPC_GPIO0->SET = 0xFF00;
		yield();
}

void p2(void)
{
		LPC_GPIO0->CLR = 0xF000;
    yield();
}

void yield()
{
		get_next_pid();
    if(procs[next_pid].sp == 0) // 0 entspricht noch nicht aufgerufen
				procs[next_pid].sp = (uintptr_t)&(stack[next_pid][31])-9*4;

		stack[next_pid][31] = (uint32_t)procs[next_pid].Func;
    ctxsw(&procs[cur_pid].sp, procs[next_pid].sp);
}

void get_next_pid(void)
{
    if (tick % 2 == 0)
        next_pid = 1;
    else
        next_pid = 0;
    tick++;
}

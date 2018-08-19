#ifndef EBSY
#include <LPC177x_8x.h>
#include <stdint.h>
#include <stdlib.h>
#include "ctxsw.h"
#include "fork.h"
#include "helpers.h"
#define EBSY

#define MAXPROCS 255
#define STACKMAX 32
#define TENMS 0x61A7F

/* This is a task in our 'Process-Table' */
typedef struct {
		int32_t pid;
		uintptr_t sp;
		uint32_t Interval;
		uint32_t LastTick;
		void (*Func)(void);
} proc;

/* enum { UNUSED=-1, READY, RUNNING, WAITING, ZOMBIE } */

/* function definitions */
void p1(void);
void p2(void);
void p3(void);
void yield(void);
void get_next_pid(void);

/* Process-Table */
extern proc procs[MAXPROCS];

/* various other globals */
extern uint32_t cur_pid;
extern uint32_t next_pid;
extern uint8_t numProcs;
extern uint32_t stack[MAXPROCS][STACKMAX];
extern uintptr_t cur_sp, next_sp;
extern volatile uintptr_t SysTick_CTRL;
#endif

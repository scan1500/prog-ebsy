#include <LPC177x_8x.h>
#include <stdint.h>
#include "ebsy.h"

#ifndef CUST_HANDLERS
    #include "handlers.h"
#endif

/*  
    globals
    process table. I fucked this one up pretty bad at first. Had multiple declarations, because I used 'static' wrong.
*/
proc procs[] = {
        {1,0x42,1,0,p1},
        {2,0x23,1,0,p2},
        {3,0x77,1,0,p3},
};

uint32_t cur_pid = 1;
uint32_t next_pid = 1;
uintptr_t cur_sp, next_sp;
//uint8_t numProcs = sizeof(procs) / sizeof(*procs);
uint8_t numProcs = 3;
uint32_t stack[MAXPROCS][STACKMAX];
volatile uintptr_t SysTick_CTRL;


int main(void)
{  
    /* Interrupt priorities */
    SCB->SHP[10] = 0x3; // PendSV Priority Level
    SCB->SHP[11] = 0; // SysTick Priority Level
    
    /* GPIO setup */
    LPC_GPIO0->DIR = 0xFF00;
/*
    stack[cur_pid][31] = (uint32_t)0x21000000;                      // xPSR
    stack[cur_pid][30] = (uintptr_t)procs[cur_pid].Func;            // PC
    stack[cur_pid][29] = (uintptr_t)procs[cur_pid].Func;            // LR
    stack[cur_pid][28] = 0xdeadbeef;                                // R12
    stack[cur_pid][27] = 0;                                         // R3   
    stack[cur_pid][26] = 0;                                         // R2
    stack[cur_pid][25] = 0;                                         // R1
    stack[cur_pid][24] = 0;                                         // R0
    stack[cur_pid][23] = 0;                                         // R11
    stack[cur_pid][22] = 0;                                         // R10
    stack[cur_pid][21] = 0;                                         // R9
    stack[cur_pid][20] = 0;                                         // R8
    stack[cur_pid][19] = 0;                                         // R7
    stack[cur_pid][18] = 0;                                         // R6
    stack[cur_pid][17] = 0;                                         // R5
    stack[cur_pid][16] = 0;                                         // R4
*/
    stack[cur_pid-1][31] = (uintptr_t)procs[cur_pid-1].Func;            // Entry point
    procs[cur_pid-1].sp = (uintptr_t)&(stack[cur_pid-1][31]);

    /* initial stack-setup for all procs */
    for(int proc_count = 1; proc_count < numProcs; proc_count++) {
        stack[proc_count][31] = (uint32_t)0x21000000;               // xPSR
        stack[proc_count][30] = (uintptr_t)procs[proc_count].Func;  // PC
        stack[proc_count][29] = (uintptr_t)procs[proc_count].Func;  // LR
        stack[proc_count][28] = 0xdeadbeef;                         // R12
        stack[proc_count][27] = 0;                                  // R3
        stack[proc_count][26] = 0;                                  // R2
        stack[proc_count][25] = 0;                                  // R1
        stack[proc_count][24] = 0;                                  // R0
        stack[proc_count][23] = 0;                                  // R11
        stack[proc_count][22] = 0;                                  // R10
        stack[proc_count][21] = 0;                                  // R9
        stack[proc_count][20] = 0;                                  // R8
        stack[proc_count][19] = 0;                                  // R7
        stack[proc_count][18] = 0;                                  // R6
        stack[proc_count][17] = 0;                                  // R5
        stack[proc_count][16] = 0;                                  // R4
        procs[proc_count].sp = (uintptr_t)&(stack[proc_count][16]); // reserve space on stack
    }
    

/*    
    SysTick Config 
    
    easy way using CMSIS (not allowed)
    volatile uint32_t clk = SystemCoreClock;
    SysTick_Config(SystemCoreClock / 1000);
*/
    
    // real deal, loading the register by ourself
    //SysTick->LOAD = TENMS;    // prod value
    //SysTick->LOAD = 0xFF1A7F;   // using this value for debugging
    SysTick->LOAD = 0x0FFFFF; 
    SysTick->VAL = 0;
    SysTick_CTRL = (uintptr_t)&(SysTick->CTRL);
    
    // would enable the SysTick-Timer. We're not doing it here, but in firstProc. Otherwise we'd get the first timer-interrupt while still in firstProc, which we obviously don't want.
    //SysTick->CTRL |= 0x7;
    
    // actually setup first process -> ctxsw.s (yep, this is an assembler-routine)
    firstProc(procs[cur_pid-1].sp);
}

// processes

void p1(void)
{
    while(1)
        LPC_GPIO0->CLR = 0xF000;
}

void p2(void)
{
    int32_t child = 0x42;
    child = fork();
    if(child == 0) {
        while(1) {
            LPC_GPIO0->CLR = 0xFFFF;
            LPC_GPIO0->SET = 0xFF00;
        }
    } else {
        while(1) {
            LPC_GPIO0->CLR = 0xFFFF;
            LPC_GPIO0->SET = 0x00FF;
        }
    }
    
}

void p3(void)
{
    while(1)
        LPC_GPIO0->SET = 0x8000;
    
}

// keeping this, for later
/*void yield()
{
        if(procs[next_pid].sp == 0)
        {
            procs[next_pid].sp = (uintptr_t)&(stack[cur_pid+1][31])-9*4;
        }
        stack[next_pid][31] = (uint32_t)procs[next_pid].Func;
}*/

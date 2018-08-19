#include "ebsy.h"
#include "fork.h"
//#include "helpers.h"

int fork() {
	__asm {
        svc #1
    }
		return get_r0();
}

int sys_fork(void) {
    interrupts_disable(); // we want no interrupts, while creating a new process
    
    int new_pid;
    new_pid = get_new_pid();
    copy_stack(new_pid);
    create_new_proc(new_pid);
    numProcs++;
    
    interrupts_enable(); // switch interrupts back on
    return new_pid;
}

int get_new_pid(void) {
    int new_pid = 0;
    
    for(int i = 0; i < MAXPROCS-1;i++) {
        if (procs[i].pid == 0) {
            new_pid = i+1;
            return new_pid;
        }
    }
    return new_pid;
}

void copy_stack(int new_pid) {
    for(int i = 0; i < STACKMAX; i++) {
        stack[new_pid-1][i] = stack[cur_pid-1][i];
    }
    /* This should actually be stack[xy][24]. Gotta find out, why this is going wrong */
	stack[new_pid-1][21] = 0; // set R0 to 0, so we can distinguish parent and child
    stack[cur_pid-1][21] = new_pid; // set R0 to cur_pid, to return the child's pid in fork()
}


void create_new_proc(int new_pid) {
    /* TODO
     * create a new proc_struct with
     * pid = new_pid
     * sp = pointer to copied stack
     */
		uintptr_t sp_offset = 0;
    procs[new_pid-1].Func = procs[cur_pid-1].Func;
    procs[new_pid-1].Interval = procs[cur_pid-1].Interval;
    procs[new_pid-1].LastTick = 0;
    procs[new_pid-1].pid = new_pid;
    /*
     * calculate the correct sp by subtracting the original sp's offset from the head of our new stack
     */
		sp_offset = (char*)&(stack[cur_pid-1][32]) - (char*)procs[cur_pid-1].sp;
    procs[new_pid-1].sp = (uintptr_t)((char*)(&(stack[new_pid-1][32])) - sp_offset - 32);
}

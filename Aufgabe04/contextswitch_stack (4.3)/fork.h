#ifndef FORK
#define FORK

#include "helpers.h"

int sys_fork(void);
int fork(void);
int get_new_pid(void);
void copy_stack(int32_t);
void create_new_proc(int32_t);

#endif

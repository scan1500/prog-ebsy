#ifndef HELPERS
#define HELPERS

extern void interrupts_enable(void);
extern void interrupts_disable(void);
void update_cur_sp(uintptr_t);
int get_r0(void);

#endif

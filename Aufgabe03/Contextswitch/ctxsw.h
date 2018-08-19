#define CTXSW

extern void firstProc(uintptr_t sp);
extern void ctxsw(uintptr_t* old_stack, uintptr_t new_stack);

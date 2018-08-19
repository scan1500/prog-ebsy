#include "ebsy.h"

void update_cur_sp(uintptr_t sp) {
	procs[cur_pid-1].sp = sp;
}

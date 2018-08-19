#include "fork.h"
#define SYS_fork 1
#define SYS_open 2
#define SYS_close 3
#define SYS_read 4
#define SYS_write 5
#define SYS_ioctl 6

int SVCHandler_main(unsigned int *);

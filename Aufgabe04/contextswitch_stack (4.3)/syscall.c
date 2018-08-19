#include "ebsy.h"
#include "syscall.h"

//extern int sys_fork(void);
/* extern int sys_open(void); */
/* extern int sys_close(void); */
/* extern int sys_read(void); */
/* extern int sys_write(void); */
/* extern int sys_ioctl(void); */

static int (*syscalls[])(void) = {
	[SYS_fork]	= sys_fork,
	/* [SYS_open]	= sys_open, */
	/* [SYS_close]	= sys_close, */
	/* [SYS_read]	= sys_read, */
	/* [SYS_write]	= sys_write, */
	/* [SYS_ioctl]	= sys_ioctl, */
};

int SVCHandler_main(unsigned int *svc_args) {
	int ret = 0;
    unsigned int svc_number;
    /*
     * Stack contains:
     * r0, r1, r2, r3, r12, lr, return address and xPSR
     * First argument (r0) is svc_args[0]
     */
    svc_number = ((char *)svc_args[6])[-2];
	ret = syscalls[svc_number]();
    /* switch(svc_number) { */
    /*     case SVC_00: */
    /*         /1* TODO: Handle SVC 00 *1/ */
    /*         ret = SYS_fork(); */
    /*         break; */
    /*     case SVC_01: */
    /*         /1* TODO: Handle SVC 01 *1/ */
    /*         break; */
    /*     default: */
    /*         /1* Unknown SVC *1/ */
    /*         break; */
    /* } */
	return ret;
}

#include "ebsy.h"
#include "syscall.h"

int SVCHandler_main(unsigned int *svc_args) {
	int ret = 0;
    unsigned int svc_number;
    /*
     * Stack contains:
     * r0, r1, r2, r3, r12, lr, return address and xPSR
     * First argument (r0) is svc_args[0]
     */
    svc_number = ((char *)svc_args[6])[-2];
    switch(svc_number) {
        case SVC_00:
            /* TODO: Handle SVC 00 */
            ret = SYS_fork();
            break;
        case SVC_01:
            /* TODO: Handle SVC 01 */
            break;
        default:
            /* Unknown SVC */
            break;
    }
	return ret;
}

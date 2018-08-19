#include <stdint.h>
#include <stdlib.h>
#include "ebsy.h"

// round robin
void get_next_pid(void)
{
    if (next_pid < (numProcs) ) {
        next_pid++;
    } else {
        next_pid = 1;
    }
}


//    while(1) {
//        for (TaskIdx = 0; TaskIdx < numProcs; TaskIdx++) {
//            tick++;
//            if((tick - TaskPtr[TaskIdx].LastTick) >= TaskPtr[TaskIdx].Interval) {
//                (*TaskPtr[TaskIdx].Func)();
//                TaskPtr[TaskIdx].LastTick = tick;
//            }
//        }
//    }

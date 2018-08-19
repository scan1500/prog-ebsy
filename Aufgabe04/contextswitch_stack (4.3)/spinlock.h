#include "ebsy.h"

typedef uint8_t lock; 

extern void acquire(lock *);
extern void release(lock *);

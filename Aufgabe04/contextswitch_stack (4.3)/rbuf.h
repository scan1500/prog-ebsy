#ifndef RBUF
#include <stdint.h>
#include <stdlib.h>
#define RBUF_SIZE 256

typedef struct ringBuf{
	uint8_t buf[RBUF_SIZE];
	int head;
	int tail;
	int count;
} ringBuf;

void ringBuf_init(ringBuf *buf);
int ringBuf_empty(ringBuf *buf);
int ringBuf_full(ringBuf *buf);
int ringBuf_get(ringBuf *buf);
void ringBuf_put(ringBuf *buf, const unsigned char c);
void ringBuf_flush(ringBuf *buf, const int clearBuffer);

#endif

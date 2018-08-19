#include "rbuf.h"

void ringBuf_init(ringBuf *buf) {
	buf->head = 0;
	buf->tail = 0;
	buf->count = 0;
}

int ringBuf_empty(ringBuf *buf) {
	if(buf->count == 0)
		return 1;
	else
		return 0;
}

int ringBuf_full(ringBuf *buf) {
	if(buf->count == RBUF_SIZE)
		return 1;
	else
		return 0;
}

int ringBuf_get(ringBuf *buf) {
	if(!ringBuf_empty(buf)) {
		int c =buf->buf[buf->tail];
		buf->tail = ( buf->tail+1 ) % RBUF_SIZE;
		buf->count--;
		return c;
	} else {
		return -1;
	}
}

void ringBuf_put(ringBuf *buf, const unsigned char c) {
	if(!ringBuf_full(buf)) {
		buf->buf[buf->head] = c;
		buf->head = (buf->head+1) % RBUF_SIZE ;
		buf->count++;
	}
}

void ringBuf_flush(ringBuf *buf, const int clearBuffer){
}

#include "trap.h"

typedef unsigned char uint8_t;

int main() {
  	extern char _heap_start, _heap_end;
	int heap_start = (int)&_heap_start;
	int heap_end = (int)&_heap_end;
	for(uint32_t i=heap_start; i<heap_end; i+=4) {
		volatile uint32_t* p=(volatile uint32_t*)i;
		*p = i;
	}
	for(uint32_t i=heap_start; i<heap_end; i+=4) {
		volatile uint32_t* p=(volatile uint32_t*)i;
		check(*p == i);
	}
	for(uint32_t i=heap_start; i<heap_end; i+=2) {
		volatile uint16_t* p=(volatile uint16_t*)i;
		*p = (i&0xffff);
	}
	for(uint32_t i=heap_start; i<heap_end; i+=2) {
		volatile uint16_t* p=(volatile uint16_t*)i;
		check(*p == (i&0xffff));
	}
	for(uint32_t i=heap_start; i<heap_end; i++) {
		volatile uint8_t* p=(volatile uint8_t*)i;
		*p = (i&0xff);
	}
	for(uint32_t i=heap_start; i<heap_end; i++) {
		volatile uint8_t* p=(volatile uint8_t*)i;
		check(*p == (i&0xff));
	}
	return 0;
}

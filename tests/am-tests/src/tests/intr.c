#include <amtest.h>
#include <stdio.h>

Context *simple_trap(Event ev, Context *ctx) {
  switch(ev.event) {
    case EVENT_IRQ_TIMER:
      putch('t'); putch('\n'); break;
    case EVENT_IRQ_IODEV:
      putch('d'); putch('\n'); break;
    case EVENT_YIELD:
      putch('y'); putch('\n'); break;
	case EVENT_ERROR:
      panic("EVENT_ERROR handled!\n"); break;
    default:
	  printf("%d\n", ev.event);
      panic("Unhandled event\n"); break;
  }
  return ctx;
}

void hello_intr() {
  printf("Hello, AM World @ " __ISA__ "\n");
  printf("  t = timer, d = device, y = yield\n");
  io_read(AM_INPUT_CONFIG);
  iset(1);
  while (1) {
    for (volatile int i = 0; i < 10000; i++) ;
    yield();
  }
}

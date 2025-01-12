#include "trap.h"
#define SPI_MASTER 0x10001000L
uint32_t * flash_addr = (uint32_t *)0x30000000L;


static inline uint8_t  inb(uintptr_t addr) { return *(volatile uint8_t  *)addr; }
static inline uint32_t inl(uintptr_t addr) { return *(volatile uint32_t *)addr; }
static inline void outl(uintptr_t addr, uint32_t data) { *(volatile uint32_t *)addr = data; }
static inline void outb(uintptr_t addr, uint8_t  data) { *(volatile uint8_t  *)addr = data; }

int main(){
    for(int i = 0; i < 20; i++) {
        uint32_t data_flash = inb(0x30000000L + i);
        printf("%d %d\n",i,(int)data_flash);
        if(data_flash!=i*3) {
            halt(i+100);
        }
    }
    return 0;
}
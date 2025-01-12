#include "trap.h"

typedef unsigned char uint8_t;
typedef unsigned uint32_t;

#define SPI_BASE 0x10001000
#define SPI_TX0   (SPI_BASE + 0x04)
#define SPI_TX1   (SPI_BASE + 0x04)
#define SPI_RX   (SPI_BASE + 0x00)
#define SPI_CTRL (SPI_BASE + 0x10)
#define SPI_DIV  (SPI_BASE + 0x14)
#define SPI_SS   (SPI_BASE + 0x18)
#define CHAR_LEN_OFFSET 0
#define RX_NEG_OFFSET 9
#define TX_NEG_OFFSET 10
#define LSB_OFFSET 11
#define ASS_OFFSET 13
#define BUSY_BIT 8
#define START_BIT 8


// 假设有以下宏定义
#define SPI_SS_BITREV  (1 << 7)

uint32_t flash_read(uint32_t address) {
    // 1. 选择flash模块作为SPI slave
    *((volatile uint32_t*)SPI_SS) = 1;

    // 2. 配置SPI控制寄存器
    *((volatile uint32_t*)SPI_CTRL) = (64 << CHAR_LEN_OFFSET) | (0 << RX_NEG_OFFSET) | (1 << TX_NEG_OFFSET) | (0 << LSB_OFFSET) | (1 << ASS_OFFSET);

    *((volatile uint32_t*)SPI_TX0) = 0;
    *((volatile uint32_t*)SPI_TX1) = (3<<24) + (address & 0xffffff);

    // 5. 启动传输
    *((volatile uint32_t*)SPI_CTRL) = (1 << START_BIT) | (64 << CHAR_LEN_OFFSET) | (0 << RX_NEG_OFFSET) | (1 << TX_NEG_OFFSET) | (0 << LSB_OFFSET) | (1 << ASS_OFFSET);

    // 6. 轮询传输完成
    while (*((volatile uint32_t*)SPI_CTRL) & (1 << BUSY_BIT));

    // 7. 读取接收数据
    uint32_t result = *((volatile uint32_t*)SPI_RX);

    return result;
}
int main() {
    for(int i=0; i<24; i+=4) {
        uint32_t res= flash_read(i);
        int x3=res%256;
        res/=256;
        int x2=res%256;
        res/=256;
        int x1=res%256;
        res/=256;
        printf("%d %d %d %d\n",res,x1,x2,x3);
    }
    return 0;
}
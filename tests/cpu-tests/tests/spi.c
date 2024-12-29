#include "trap.h"

typedef unsigned char uint8_t;

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
#define SPI_SS_BITREV  (1 << 6)

void bitrev_test(uint16_t data) {
    // 1. 选择bitrev模块作为SPI slave
    *((volatile uint32_t*)SPI_SS) = SPI_SS_BITREV;

    // 2. 配置SPI控制寄存器
    // CHAR_LEN = 16 (8位发送 + 8位接收)
    // 其他字段根据约定设置，例如：
    // Rx_NEG = 0, Tx_NEG = 0, LSB = 0
    *((volatile uint32_t*)SPI_CTRL) = (16 << CHAR_LEN_OFFSET) | (0 << RX_NEG_OFFSET) | (0 << TX_NEG_OFFSET) | (0 << LSB_OFFSET);

    // 3. 设置SPI时钟分频
    *((volatile uint32_t*)SPI_DIV) = 1; // 尽量高的频率

    // 4. 写入发送数据（高8位为命令，低8位为数据）
    *((volatile uint32_t*)SPI_TX0) = ((uint16_t)data);

    // 5. 启动传输
    *((volatile uint32_t*)SPI_CTRL) |= (1 << START_BIT);

    // 6. 轮询传输完成
    while (*((volatile uint32_t*)SPI_CTRL) & (1 << BUSY_BIT));

    // 7. 读取接收数据
    uint8_t result = *((volatile uint32_t*)SPI_RX) & 0xFF;

    // 8. 检查结果
    // 例如，打印或断言
    printf("result: %d\n", (int)result);
}
int main() {
    bitrev_test(0b1111000011110000);
    return 0;
}
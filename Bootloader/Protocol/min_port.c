#include "min.h"
#include "uart_port.h"
#include "stm32f1xx_hal.h"
extern UART_HandleTypeDef huart2;
uint32_t min_time_ms(void)
{
    return HAL_GetTick();
}

uint16_t min_tx_space(uint8_t port)
{
    return 512;
}

void min_tx_byte(uint8_t port, uint8_t byte)
{
    HAL_UART_Transmit(&huart2, &byte, 1, 1000);
}

void min_tx_start(uint8_t port) {}
void min_tx_finished(uint8_t port) {}

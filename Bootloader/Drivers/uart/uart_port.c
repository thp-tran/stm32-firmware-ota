#include "uart_port.h"
#include "circular_queue.h"
#include "stm32f1xx_hal.h"

#define UART_RX_BUF_SIZE 256

extern UART_HandleTypeDef huart1;

static uint8_t rx_byte;
static uint8_t rx_buf[UART_RX_BUF_SIZE];
static circular_queue_t rx_queue;

void uart_port_init(void)
{
    cq_init(&rx_queue, rx_buf, UART_RX_BUF_SIZE);
    HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        cq_push(&rx_queue, rx_byte);
        HAL_UART_Receive_IT(&huart1, &rx_byte, 1);
    }
}

uint8_t uart_port_read(uint8_t *data)
{
    return cq_pop(&rx_queue, data);
}

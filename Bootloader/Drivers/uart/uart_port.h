#pragma once
#include <stdint.h>
#include <usart.h>
void uart_port_init(void);
uint8_t uart_port_read(uint8_t *data);

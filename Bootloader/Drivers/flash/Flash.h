#ifndef FLASH_H
#define FLASH_H
#include "stm32f1xx_hal.h"

void erase_flash(uint32_t addr);
void lock_flash(void);
void unlock_flash(void);
void write_flash(uint32_t addr, uint8_t* mData, uint16_t len);
void erase_app(void);
void erase_page(uint8_t page_addr);
#endif
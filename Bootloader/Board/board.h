#pragma once

#define ADDR_APP 0x0800C800
#define FLASH_PAGE_SIZE 1024
#define META_DATA_ADDR 0x0801FC00
#define UPDATE_FLAG_ADDR 0x0801FFF0
#define UPDATE_FLAG (*((volatile uint32_t*) (UPDATE_FLAG_ADDR)))
void board_jump_to_app(void);

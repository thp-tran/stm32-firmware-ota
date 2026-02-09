#include "Flash.h"
#include "board.h"
void erase_flash(uint32_t addr){
	FLASH_EraseInitTypeDef pEraseInit;
	pEraseInit.NbPages = 1;
	pEraseInit.PageAddress = addr;
	pEraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
	uint32_t PageError;
	HAL_FLASHEx_Erase(&pEraseInit, &PageError);
}

void erase_app(void){
	for(uint16_t i = 0; i < 50; i++){
		erase_flash(ADDR_APP + i * 1024);
	}
}

void unlock_flash(void){
	HAL_FLASH_Unlock();
}

void lock_flash(void){
	HAL_FLASH_Lock();
}

void write_flash(uint32_t addr, uint8_t* mData, uint16_t len){
	for(uint16_t i = 0; i < len; i += 2){
		if(i + 1 >= len) mData[i + 1] = 0xFF;
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, addr + i, (uint16_t)(mData[i + 1] << 8) | mData[i]);
	}
}
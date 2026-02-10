#include "board.h"
#include "stm32f1xx_hal.h"

/* Application entry type */
typedef void (*pFuntion)(void);

void board_jump_to_app(void)
{
		HAL_RCC_DeInit();
		HAL_DeInit();
		SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk | SCB_SHCSR_BUSFAULTENA_Msk | SCB_SHCSR_MEMFAULTENA_Msk);
		__set_MSP(*(__IO uint32_t *)ADDR_APP);
		pFuntion app_entry = (pFuntion)(*(__IO uint32_t *)(ADDR_APP + 4));
		app_entry();
}

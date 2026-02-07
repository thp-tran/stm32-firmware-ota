#include "board.h"
#include "stm32f1xx_hal.h"

/* Application entry type */
typedef void (*pFunction)(void);

void board_jump_to_app(void)
{
    uint32_t app_sp;
    uint32_t app_reset;
    pFunction app_entry;

    /* Disable all interrupts */
    __disable_irq();

    /* Deinit HAL & peripherals */
    HAL_RCC_DeInit();
    HAL_DeInit();

    /* Disable SysTick */
    SysTick->CTRL = 0;
    SysTick->LOAD = 0;
    SysTick->VAL  = 0;

    /* Clear fault handlers */
    SCB->SHCSR &= ~(SCB_SHCSR_USGFAULTENA_Msk |
                    SCB_SHCSR_BUSFAULTENA_Msk |
                    SCB_SHCSR_MEMFAULTENA_Msk);

    /* Get application's stack pointer & reset vector */
    app_sp    = *(__IO uint32_t *)ADDR_APP;
    app_reset = *(__IO uint32_t *)(ADDR_APP + 4);

    /* Set MSP */
    __set_MSP(app_sp);

    /* Jump to application */
    app_entry = (pFunction)app_reset;
    app_entry();
}

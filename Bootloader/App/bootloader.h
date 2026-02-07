#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "min.h"

/* ===== Bootloader OTA states ===== */
typedef enum
{
    BL_IDLE = 0,
    BL_RECEIVING,
    BL_VERIFY,
    BL_COMMIT,
    BL_ROLLBACK,
    BL_JUMP_APP
} bl_state_t;

/* ===== Public API ===== */
void bootloader_init(void);
void bootloader_process(void);

/* MIN callback */
void min_application_handler(uint8_t min_id,
                             uint8_t const *payload,
                             uint8_t len,
                             uint8_t port);

/* Query state */
bool bootloader_is_done(void);

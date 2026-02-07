#include "bootloader.h"
#include "ota_protocol.h"
#include "flash.h"
#include "board.h" // ADDR_APP, FLASH_PAGE_SIZE, etc.
#include "stm32f1xx_hal.h"
#include <string.h>

/* ===== CONFIG ===== */
#define OTA_TIMEOUT_MS 5000
#define OTA_MAX_SIZE (64 * 1024)

/* ===== Static variables ===== */
static bl_state_t bl_state = BL_IDLE;
static uint32_t last_activity_ms = 0;
static uint32_t write_addr = ADDR_APP;
static uint32_t received_size = 0;
static bool ota_done_flag = false;

static struct min_context min_ctx;

/* ===== Internal helpers ===== */
static void bl_reset_ota(void)
{
    write_addr = ADDR_APP;
    received_size = 0;
    last_activity_ms = HAL_GetTick();
}

/* ===== Public functions ===== */
void bootloader_init(void)
{
    min_init_context(&min_ctx, 0);
    bl_state = BL_IDLE;
    ota_done_flag = false;
}

bool bootloader_is_done(void)
{
    return ota_done_flag;
}

/* ===== Main state machine ===== */
void bootloader_process(void)
{
    uint32_t now = HAL_GetTick();

    switch (bl_state)
    {
    case BL_IDLE:
        /* waiting for START_OTA */
        break;

    case BL_RECEIVING:
        if (now - last_activity_ms > OTA_TIMEOUT_MS)
        {
            bl_state = BL_ROLLBACK;
        }
        break;

    case BL_VERIFY:
        if (ota_verify_image(ADDR_APP, received_size))
        {
            bl_state = BL_COMMIT;
        }
        else
        {
            bl_state = BL_ROLLBACK;
        }
        break;

    case BL_COMMIT:
        ota_mark_image_valid();
        bl_state = BL_JUMP_APP;
        break;

    case BL_ROLLBACK:
        ota_rollback();
        bl_state = BL_JUMP_APP;
        break;

    case BL_JUMP_APP:
        ota_done_flag = true;
        board_jump_to_app();
        break;

    default:
        break;
    }
}

/* ===== MIN application callback ===== */
void min_application_handler(uint8_t min_id,
                             uint8_t const *payload,
                             uint8_t len,
                             uint8_t port)
{
    (void)port;

    last_activity_ms = HAL_GetTick();

    switch (min_id)
    {
    case OTA_CMD_START:
        //flash_erase_app();
        bl_reset_ota();
        bl_state = BL_RECEIVING;
        ota_send_ack(&min_ctx);
        break;

    case OTA_CMD_DATA:
        if (bl_state != BL_RECEIVING)
            break;

        if (received_size + len > OTA_MAX_SIZE)
        {
            bl_state = BL_ROLLBACK;
            break;
        }

        write_flash(write_addr, (uint8_t *)payload, len);
        write_addr += len;
        received_size += len;

        ota_send_ack(&min_ctx);
        break;

    case OTA_CMD_END:
        bl_state = BL_VERIFY;
        ota_send_ack(&min_ctx);
        break;

    default:
        break;
    }
}

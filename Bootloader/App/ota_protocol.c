#include "ota_protocol.h"
#include "board.h"      // ADDR_APP
#include <string.h>
/*
 * OTA ACK frame payload:
 * [0] = status
 */

void ota_send_ack(struct min_context *ctx)
{
    uint8_t status = OTA_ACK_OK;
    min_send_frame(ctx, OTA_CMD_ACK, &status, 1);
}

void ota_send_nack(struct min_context *ctx)
{
    uint8_t status = OTA_ACK_ERR;
    min_send_frame(ctx, OTA_CMD_ACK, &status, 1);
}

/*
 * Very basic image verification:
 *  - stack pointer inside SRAM
 *  - reset vector inside flash
 */
bool ota_verify_image(uint32_t addr, uint32_t size)
{
    if (size < 8)
        return false;

    uint32_t sp = *(uint32_t *)addr;
    uint32_t rv = *(uint32_t *)(addr + 4);

    /* STM32F103 SRAM: 0x2000 0000 - 0x2001 FFFF */
    if ((sp & 0x2FFE0000) != 0x20000000)
        return false;

    /* Flash address check */
    if (rv < addr || rv > (addr + size))
        return false;

    return true;
}

/*
 * Mark image valid
 * (placeholder – real project would store metadata in flash)
 */
void ota_mark_image_valid(void)
{
    /* TODO:
     * write magic word / version to metadata page
     */
}

/*
 * Rollback handler
 */
void ota_rollback(void)
{
    /* TODO:
     * restore previous image
     * or stay in bootloader
     */
}

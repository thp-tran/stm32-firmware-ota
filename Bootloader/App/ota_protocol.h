#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "min.h"

/* ===== MIN IDs used for OTA ===== */
#define OTA_CMD_START   0x01
#define OTA_CMD_DATA    0x02
#define OTA_CMD_END     0x03
#define OTA_CMD_ACK     0x7F   // response from device

/* ===== ACK status ===== */
typedef enum
{
    OTA_ACK_OK = 0x00,
    OTA_ACK_ERR = 0x01
} ota_ack_t;

/* ===== Public API ===== */

/* Send ACK/NACK to host */
void ota_send_ack(struct min_context *ctx);
void ota_send_nack(struct min_context *ctx);

/* Verify received firmware image */
bool ota_verify_image(uint32_t addr, uint32_t size);

/* Mark image valid (for rollback support) */
void ota_mark_image_valid(void);

/* Rollback handler */
void ota_rollback(void);

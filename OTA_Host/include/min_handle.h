#ifndef MIN_HANDLE_H
#define MIN_HANDLE_H
#include "global.h"

typedef struct
{
    uint8_t id;
    uint8_t status;
} ota_response_t;

typedef struct
{
    uint8_t id;
    uint8_t length;
    uint8_t data[21];
} ota_data_t;

typedef struct
{
    uint8_t id;
    uint8_t code;
} ota_command_t;

typedef enum
{
    SEND_DATA,
    END
} OTA_STATE;

typedef enum
{
    OTA_ACK = 0x00,
    OTA_ACK_ERR = 0x01,
} RESPONSE_STATUS;

typedef enum
{
    START_OTA = 0x00,
    SEND_OTA_DATA = 0x01,
    END_OTA = 0x02,
    ERASE_OTA = 0x03,
} OTA_CODE;

typedef enum
{
    COMMAND = 0x01,
    DATA = 0x02,
    RESPONSE = 0x03,
} SEND_TYPE;
#define OTA_CMD_START 0x01
#define OTA_CMD_DATA 0x02
#define OTA_CMD_END 0x03
#define OTA_CMD_ACK 0x04 // response from device
void min_init();
void ota_send_start();
void min_rx_data(void *pvParameters);
void send_cmd(ota_command_t cmd);
void send_data(uint8_t *payload, uint8_t size, uint8_t min_id);
#endif // MIN_HANDLE_H
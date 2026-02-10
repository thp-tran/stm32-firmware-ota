#include "min_handle.h"
HardwareSerial min_serial(1);
min_context min_ctx;
OTA_STATE ota_state = IDLE;
void min_init()
{
    min_serial.begin(115200, SERIAL_8N1, 17, 16);
    min_init_context(&min_ctx, 0);
}

uint32_t min_time_ms(void)
{
    return millis();
}

uint16_t min_tx_space(uint8_t port)
{
    return 512;
}

void min_tx_byte(uint8_t port, uint8_t byte)
{
    min_serial.write(byte);
}

void min_tx_start(uint8_t port)
{
    // Optional: Implement if needed
}

void min_tx_finished(uint8_t port)
{
    // Optional: Implement if needed
}

void min_rx_data(void *pvParameters)
{
    uint8_t data;
    uint8_t len;
    while (1)
    {
        if (min_serial.available())
        {
            data = min_serial.read();
            len = 1;
        }
        min_poll(&min_ctx, &data, 1);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void ota_send_start()
{
    ota_command_t cmd;
    cmd.id = COMMAND; // Example command ID for OTA start
    cmd.code = START_OTA;
    Serial.print("Send start code");
    Serial.println();
    min_send_frame(&min_ctx, OTA_CMD_START, (uint8_t *)&cmd, sizeof(cmd));
    ota_state = SEND_DATA;
}

void send_data(uint8_t *payload, uint8_t size, uint8_t min_id)
{
    min_send_frame(&min_ctx, min_id, (uint8_t *)payload, size);
}

void send_cmd(uint8_t min_id)
{
    uint8_t dummy_data = 0xFF;
    min_send_frame(&min_ctx, min_id, &dummy_data, sizeof(dummy_data));
}

void set_ota_idle_state(void)
{
    ota_state = IDLE;
}

void min_application_handler(uint8_t min_id, uint8_t const *min_payload, uint8_t len_payload, uint8_t port)
{
    switch (ota_state)
    {
    case SEND_DATA:
    {
        // ota_response_t *respo = (ota_response_t *)min_payload;
        uint8_t *respo = (uint8_t *)min_payload;
        if (min_id == OTA_CMD_ACK) // Example command ID for OTA data ACK
        {
            Serial.println("Received ACK for OTA data packet.");
            isACK = true;
            xSemaphoreGive(xBinarySemaphoreAck);
        }
        break;
    }
    case END:
        break;
    default:
        break;
    }
}
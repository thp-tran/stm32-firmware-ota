#include "coreiot.h"
#include <WiFiClientSecure.h>
WiFiClient CoreIoTWifiClient;
WiFiClient HttpWifiClient;
PubSubClient client(CoreIoTWifiClient);
WiFiClientSecure client_sec;
HTTPClient https;
// Url for downloading STM32 lastest firmware
char fw_url[256];
uint8_t transmit_data[21];
void processUrl(byte *payload, unsigned int length)
{
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }
    if (doc["fw_title"] && doc["fw_version"])
    {
        const char *fw_title = doc["fw_title"];
        const char *fw_version = doc["fw_version"];
        snprintf(fw_url, sizeof(fw_url), "%stitle=%s&version=%s", base_url, fw_title, fw_version);
    }
    return;
}

uint8_t char_to_byte(char c)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 10;
    else if (c >= 'a' && c <= 'f')
        return c - 'a' + 10;
    return 0;
}

void convert(uint8_t *input, uint8_t *output)
{
    uint8_t index = 1;
    while (input[index] != '\n' || input[index] != '\0')
    {
        uint8_t highNibble = char_to_byte(input[index]);
        index++;
        uint8_t lowNibble = char_to_byte(input[index]);
        index++;
        output[(index / 2) - 1] = (highNibble << 4) | lowNibble;
        if (input[index] == '\n' || input[index] == '\0')
            break;
    }
}

void download_data()
{
    client_sec.setInsecure(); // test nhanh, bỏ kiểm tra SSL
    https.begin(
        client_sec,
        fw_url);
    int httpCode = https.GET();
    uint8_t lineBuffer[128];
    if (httpCode == 200)
    {
        Serial.print("Find!!!");
        Serial.println();
        WiFiClient *stream = https.getStreamPtr();
        ota_send_start();
        while (stream->available())
        {
            Serial.print("Wait for ack!!!");
            Serial.println();
            if (xSemaphoreTake(xBinarySemaphoreAck,
                               pdMS_TO_TICKS(5000)) == pdTRUE)
            {
                if (isACK)
                {
                    isACK = false;
                    uint8_t len = stream->readBytesUntil('\n', (char *)lineBuffer, sizeof(lineBuffer) - 1);
                    lineBuffer[len] = '\n'; // include newline
                    convert(lineBuffer, transmit_data);
                    uint8_t data[21];
                    memcpy(data, &transmit_data[4], transmit_data[0]);
                    if (transmit_data[0] == 0)
                    {
                        ota_command_t cmd;
                        cmd.id = COMMAND; // Example command ID for OTA start
                        cmd.code = END_OTA;
                        send_cmd(cmd);
                        https.end();
                        Serial.println("All data packets sent, sending OTA end command.");
                        return;
                    }
                    else
                    {
                        Serial.println("Send data");
                        send_data(data, transmit_data[0], OTA_CMD_DATA);
                    }
                }
            }
            else
            {
                Serial.println("Update failed");
                return;
            }

            vTaskDelay(10 / portTICK_PERIOD_MS); // Adjust delay as needed
        }
    }
}

void processReq(byte *payload, int length)
{
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, payload, length);
    if (error)
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return;
    }
    if (doc["method"] == "Upload" && doc["params"] == true)
    {
        Serial.print("Start download");
        Serial.println();
        download_data();
    }
}

void callback(char *topic, byte *payload, unsigned int length)
{
    COREIOT_MSG_TYPE msg_type;
    if (strstr(topic, "v1/devices/me/rpc/request/") == topic)
    {
        Serial.println("RPC Request received");
        msg_type = REQUEST;
    }
    else if (strstr(topic, "v1/devices/me/attributes") == topic)
    {
        Serial.println("Attributes message received");
        msg_type = ATTRIBUTE;
    }
    else
    {
        Serial.println("Unknown topic");
    }
    if (msg_type == REQUEST)
    {
        processReq(payload, length);
    }
    else if (msg_type == ATTRIBUTE)
    {
        processUrl(payload, length);
        Serial.println(fw_url);
    }
}

void setup_coreiot()
{
    Serial.println(" Internet OK!");
    client.setServer(mqtt_server, mqttPort);
    client.connect("ESP32Client", device_token, NULL);
    if (client.connected())
        Serial.println(" MQTT Connected!");
    client.subscribe("v1/devices/me/rpc/request/+");
    client.subscribe("v1/devices/me/attributes");
    client.subscribe("v1/devices/me/attributes/response/+");

    client.setCallback(callback);
}

void coreiot_task(void *pvParameters)
{
    setup_coreiot();
    while (1)
    {
        client.loop();
        // Add your MQTT publish/subscribe logic here
        vTaskDelay(1000 / portTICK_PERIOD_MS); // Adjust delay as needed
    }
}
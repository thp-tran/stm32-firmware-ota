#include "global.h"

bool WifiConnected = false;
const char *ssid = "Phuong";                                                           // Replace with your network SSID
const char *password = "12345678";                                                     // Replace with your network password
const char *mqtt_server = "app.coreiot.io";                                            // Replace with your MQTT server
const char *device_token = "po51znlco3yxaof5m07s";                                     // Replace with your device token
const char *base_url = "https://app.coreiot.io/api/v1/po51znlco3yxaof5m07s/firmware?"; // Replace with your base URL (htttps://<your_domain>/api/v1/<your_token>/firmware?)
const int mqttPort = 1883;
bool isACK = false;

SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();
SemaphoreHandle_t xBinarySemaphoreAck = xSemaphoreCreateBinary();
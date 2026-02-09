#include "global.h"
#include "coreiot.h"
#include "min_handle.h"

bool connectWifi()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void setup()
{
    min_init();
    Serial.begin(115200);
    connectWifi();
    xTaskCreate(
        coreiot_task,  // Function to implement the task
        "CoreIOTTask", // Name of the task
        8192,          // Stack size in words
        NULL,          // Task input parameter
        1,             // Priority of the task
        NULL);         // Task handle
    xTaskCreate(
        min_rx_data,     // Function to implement the task
        "ReceivingData", // Name of the task
        4096,            // Stack size in words
        NULL,            // Task input parameter
        1,               // Priority of the task
        NULL);           // Task handle
}
void loop()
{
}

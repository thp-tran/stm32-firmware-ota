#ifndef GLOBAL_H
#define GLOBAL_H
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <HardwareSerial.h>
#include <HTTPClient.h>
#include "min_handle.h"
#include "min.h"

extern bool WifiConnected;
extern const char *ssid;
extern const char *password;
extern SemaphoreHandle_t xBinarySemaphoreInternet;
extern SemaphoreHandle_t xBinarySemaphoreAck;
extern const char *mqtt_server;
extern const char *device_token;
extern const char *base_url;
extern const int mqttPort;
extern bool isACK;
extern uint8_t hex_file_data[1024][21];
#endif // GLOBAL_H
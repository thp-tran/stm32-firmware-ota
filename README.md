# STM32 & ESP32 OTA Update System via MIN Protocol

This project implements a complete **Over-The-Air (OTA)** firmware update solution for STM32 microcontrollers using an ESP32 as an intelligent gateway. The system leverages the **CoreIoT** platform for remote management and the **MIN (Micro-controller Interconnect Network) v2.0** protocol for robust data transmission over UART.



---

## 1. System Architecture

The architecture consists of three primary layers:
1.  **Cloud (CoreIoT Platform)**: Stores firmware builds and triggers updates via MQTT RPC commands.
2.  **Host (ESP32 Gateway)**: 
    * Connects to WiFi and maintains communication with the MQTT broker.
    * Downloads firmware files from the server via HTTPS.
    * Converts Hex strings into byte arrays and encapsulates them into MIN frames.
    * Manages flow control using semaphores to wait for ACKs from the target.
3.  **Target (STM32 Bootloader)**: 
    * Receives and decodes MIN protocol frames via UART.
    * Handles internal Flash memory operations (Erase/Write).
    * Jumps to the user application after a successful update.

---

## 2. Key Features

* **Reliable Transmission**: Uses MIN v2.0, which includes CRC32 checksums and frame stuffing to ensure data integrity over serial lines.
* **Flow Control**: Implementation of an ACK/NACK mechanism ensures the ESP32 only sends the next packet after the STM32 confirms the previous one was written successfully.
* **Efficient Buffering**: STM32 utilizes a circular queue to prevent data loss during high-speed UART reception.
* **Secure Flash Handling**: The bootloader performs a bulk erase before writing and locks the Flash memory after completion to prevent accidental corruption.

---

## 3. Frame

| Field       | Size    |
|------------|---------|
| Start Byte | 3 bytes |
| ID Control | 1 byte  |
| Length     | 1 byte  |
| Data       | N bytes |
| Checksum   | 4 bytes |


---

## 4. Source Code Structure

### STM32 (Target)
* `bootloader.c/h`: Core OTA state machine logic (IDLE, SEND_DATA, END).
* `Flash.c/h`: Low-level Flash memory interaction (Unlock, Erase, Write, Lock).
* `min.c/h`: Standard MIN v2.0 protocol implementation.
* `circular_queue.c/h`: Circular buffer implementation for the UART driver.
* `uart.c/h`: UART abstraction layer using interrupt-based reception.

### ESP32 (Host)
* `coreiot.cpp/h`: Handles MQTT connectivity, RPC command parsing, and HTTPS downloading.
* `min_handle.cpp/h`: Manages the MIN protocol context and OTA flow control.
* `main.cpp`: Orchestrates FreeRTOS tasks for Cloud communication and UART monitoring.
* `global.cpp/h`: Global configurations for WiFi, MQTT tokens, and URLs.

---

## 5. OTA Update Workflow

1.  **Initialization**: STM32 starts in Bootloader mode; ESP32 connects to WiFi and the CoreIoT MQTT broker.
2.  **Trigger**: User sends an "Upload" command from the CoreIoT Dashboard. ESP32 receives the RPC and constructs the firmware URL.
3.  **Preparation**: ESP32 sends a `START_OTA` command. STM32 erases the target Flash pages and replies with an `OTA_ACK`.
4.  **Data Transfer**: ESP32 downloads the firmware line-by-line, converts it to bytes, and sends it via MIN `DATA` frames.
5.  **Finalization**: Once complete, ESP32 sends an `END_OTA` command. STM32 sets a completion flag and executes the jump to the application at `0x800C800`.

---

## 6. Important Notes

* **Application Address**: The STM32 user application must be configured to start at `0x800C800` (defined as `ADDR_APP`).
* **Data Format**: The ESP32 expects the server to provide the firmware in a Hex-string format, which it converts during the download process.
* **Dependencies (ESP32)**:
    * `PubSubClient` (MQTT)
    * `ArduinoJson` (Command Parsing)
    * `HTTPClient` & `WiFiClientSecure` (HTTPS Download)


---

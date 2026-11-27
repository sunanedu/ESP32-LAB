#pragma once
#include <Arduino.h>

// เลือกขาตามชิป; ตรวจสกรีน/วงจรบอร์ดจริงก่อนต่อโมดูล
constexpr uint8_t DHT_PIN = 4;
#if defined(CONFIG_IDF_TARGET_ESP32S3)
// S3 ไม่ใช้ GPIO26–37 ที่เกี่ยวกับ Flash/PSRAM และไม่มี GPIO22
constexpr uint8_t RELAY_1_PIN = 16;
constexpr uint8_t RELAY_2_PIN = 17;
constexpr uint8_t I2C_SDA_PIN = 8;
constexpr uint8_t I2C_SCL_PIN = 9;
#elif defined(CONFIG_IDF_TARGET_ESP32)
// ESP32-WROOM-32 ตามวงจรหลักของคอร์ส
constexpr uint8_t RELAY_1_PIN = 26;
constexpr uint8_t RELAY_2_PIN = 27;
constexpr uint8_t I2C_SDA_PIN = 21;
constexpr uint8_t I2C_SCL_PIN = 22;
#else
#error "Lab07 supports ESP32 and ESP32-S3; define a verified pin mapping first."
#endif
constexpr uint8_t RELAY_ON = LOW;
constexpr uint8_t RELAY_OFF = HIGH;
constexpr uint8_t OLED_ADDRESS = 0x3C;
constexpr int OLED_WIDTH = 128;
constexpr int OLED_HEIGHT = 64;
constexpr int OLED_RESET = -1;

// ปลายทาง public broker และช่วงเวลาสำหรับ cooperative loop
constexpr char MQTT_HOST[] = "broker.emqx.io";
constexpr uint16_t MQTT_PORT = 1883;
constexpr unsigned long WIFI_RETRY_MS = 10000;
constexpr unsigned long MQTT_RETRY_MS = 5000;
constexpr unsigned long STATE_INTERVAL_MS = 10000;
constexpr unsigned long SENSOR_INTERVAL_MS = 5000;
constexpr unsigned long DISPLAY_INTERVAL_MS = 1000;

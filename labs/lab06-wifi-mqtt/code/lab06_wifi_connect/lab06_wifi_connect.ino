/*
 * Lab 06.1 : เชื่อมต่อ Wi-Fi แบบ STA
 * คำอธิบาย : เชื่อมต่อ แสดง IP/RSSI และลองเชื่อมใหม่เมื่อหลุดแบบ non-blocking
 * วงจร     : ใช้ ESP32 และสาย USB เท่านั้น
 * ไลบรารี  : WiFi (มากับ Arduino-ESP32 core 3.x)
 * Board    : ESP32S3 Dev Module / ESP32 Dev Module (WROOM-32), core 3.x
 */
#include <Arduino.h>
#include <WiFi.h>
#if __has_include("secrets.h")
#include "secrets.h"
#else
#include "secrets.example.h"
#endif

constexpr unsigned long RETRY_MS = 10000;
unsigned long lastRetry = 0;
wl_status_t previousStatus = WL_NO_SHIELD;

// เริ่มการเชื่อมต่อหนึ่งครั้ง แล้วให้ loop เป็นผู้กำหนดเวลาลองใหม่
void beginWiFi() {
  Serial.printf("[WIFI] connecting to %s\n", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  lastRetry = millis();
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  beginWiFi();
}

void loop() {
  // รายงานเฉพาะตอนสถานะเปลี่ยน เพื่อลดข้อความซ้ำใน Serial Monitor
  const wl_status_t status = WiFi.status();
  if (status != previousStatus) {
    previousStatus = status;
    if (status == WL_CONNECTED) {
      Serial.printf("[WIFI] connected IP=%s RSSI=%d dBm\n",
                    WiFi.localIP().toString().c_str(), WiFi.RSSI());
    } else {
      Serial.printf("[WIFI] status=%d\n", static_cast<int>(status));
    }
  }
  if (status != WL_CONNECTED && millis() - lastRetry >= RETRY_MS) beginWiFi();
}

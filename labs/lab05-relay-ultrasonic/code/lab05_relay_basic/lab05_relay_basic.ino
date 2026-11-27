/*
 * Lab 05.1 : ทดลอง Relay 2CH แบบ active LOW
 * คำอธิบาย : สลับ Relay 1 และ Relay 2 ทุก 2 วินาทีโดยไม่ block โปรแกรม
 * วงจร     : IN1 -> GPIO16, IN2 -> GPIO17 บน S3 (ESP32 เดิม 26/27), VCC -> 5V, GND ร่วมกัน; โหลด DC แรงดันต่ำเท่านั้น
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก
 * Board    : ESP32S3 Dev Module หรือ ESP32 Dev Module (Arduino-ESP32 core 3.x)
 */
#include <Arduino.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t RELAY_1_PIN = 16;
constexpr uint8_t RELAY_2_PIN = 17;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t RELAY_1_PIN = 26;
constexpr uint8_t RELAY_2_PIN = 27;
#else
#error "Use ESP32-S3 or ESP32 with the verified course pin map"
#endif
constexpr uint8_t RELAY_ON = LOW;
constexpr uint8_t RELAY_OFF = HIGH;
constexpr unsigned long INTERVAL_MS = 2000;

unsigned long lastChange = 0;
bool relay1On = false;

// เขียนสถานะโดยแปลงค่า bool เป็นลอจิก active LOW
void setRelay(uint8_t pin, bool on) {
  digitalWrite(pin, on ? RELAY_ON : RELAY_OFF);
}

void setup() {
  Serial.begin(115200);
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  setRelay(RELAY_1_PIN, false);
  setRelay(RELAY_2_PIN, false);
  Serial.println("[RELAY] เริ่มต้น: Relay ทั้งสอง OFF");
}

void loop() {
  const unsigned long now = millis();
  if (now - lastChange < INTERVAL_MS) return;
  lastChange = now;

  relay1On = !relay1On;
  setRelay(RELAY_1_PIN, relay1On);
  setRelay(RELAY_2_PIN, !relay1On);
  Serial.printf("[RELAY] CH1=%s CH2=%s\n", relay1On ? "ON" : "OFF",
                relay1On ? "OFF" : "ON");
}

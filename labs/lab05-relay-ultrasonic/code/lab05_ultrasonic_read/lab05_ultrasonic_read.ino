/*
 * Lab 05.2 : อ่านระยะจาก HC-SR04
 * คำอธิบาย : ส่ง trigger แล้วคำนวณระยะจากเวลาเดินทางของเสียง
 * วงจร     : S3 TRIG -> GPIO6, ECHO -> divider 1kΩ/2kΩ -> GPIO7; ESP32 เดิม 25/33; VCC -> 5V, GND ร่วมกัน
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก
 * Board    : ESP32S3 Dev Module หรือ ESP32 Dev Module (Arduino-ESP32 core 3.x)
 */
#include <Arduino.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t TRIG_PIN = 6;
constexpr uint8_t ECHO_PIN = 7;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t TRIG_PIN = 25;
constexpr uint8_t ECHO_PIN = 33;
#else
#error "Use ESP32-S3 or ESP32 with the verified course pin map"
#endif
constexpr unsigned long ECHO_TIMEOUT_US = 30000;
constexpr unsigned long SAMPLE_MS = 500;
unsigned long lastSample = 0;

// คืนค่าระยะเป็น cm; คืน NAN เมื่อไม่พบ echo ภายในเวลาที่กำหนด
float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) return NAN;
  return duration * 0.0343f / 2.0f;
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
}

void loop() {
  const unsigned long now = millis();
  if (now - lastSample < SAMPLE_MS) return;
  lastSample = now;

  const float distance = readDistanceCm();
  if (isnan(distance)) Serial.println("[SENSOR] timeout: ไม่พบ echo");
  else Serial.printf("[SENSOR] distance=%.1f cm\n", distance);
}

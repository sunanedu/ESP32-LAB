/*
 * Lab 05.3 : กรองระยะด้วย moving average
 * คำอธิบาย : เก็บตัวอย่างล่าสุด 5 ค่าเพื่อลดสัญญาณรบกวนของ HC-SR04
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
constexpr size_t WINDOW_SIZE = 5;
constexpr unsigned long SAMPLE_MS = 200;
float samples[WINDOW_SIZE] = {};
size_t sampleIndex = 0;
size_t sampleCount = 0;
unsigned long lastSample = 0;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration == 0 ? NAN : duration * 0.0343f / 2.0f;
}

// เพิ่มค่าล่าสุดลง circular buffer แล้วคืนค่าเฉลี่ย
float addAndAverage(float value) {
  samples[sampleIndex] = value;
  sampleIndex = (sampleIndex + 1) % WINDOW_SIZE;
  if (sampleCount < WINDOW_SIZE) sampleCount++;

  float sum = 0.0f;
  for (size_t i = 0; i < sampleCount; i++) sum += samples[i];
  return sum / sampleCount;
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

  const float raw = readDistanceCm();
  if (isnan(raw)) {
    Serial.println("[SENSOR] timeout; ไม่นำค่านี้ไปเฉลี่ย");
    return;
  }
  const float filtered = addAndAverage(raw);
  Serial.printf("[SENSOR] raw=%.1f cm average=%.1f cm samples=%u\n",
                raw, filtered, static_cast<unsigned>(sampleCount));
}

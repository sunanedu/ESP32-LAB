/*
 * Lab 8.2 : ตรวจเช็กอุปกรณ์ก่อนใช้งาน — ESP32-S3 + Ultrasonic HC-SR04
 * วงจร: VCC -> แหล่งจ่าย 5V, GND -> GND ร่วม, TRIG -> GPIO6
 * ECHO -> R1 1k -> จุด J -> GPIO7; จุด J -> R2 2k -> GND
 * ห้ามต่อ ECHO 5V เข้าขา GPIO7 ตรง
 * ไลบรารี: Arduino มากับ core ไม่ต้องติดตั้งเพิ่ม
 * Board: ESP32S3 Dev Module, Arduino-ESP32 core 3.x, Serial 115200
 */
#include <Arduino.h>

#if !defined(CONFIG_IDF_TARGET_ESP32S3)
#error "Select ESP32S3 Dev Module; this sketch uses the course ESP32-S3 pin map"
#endif

constexpr uint8_t TRIG_PIN = 6;
constexpr uint8_t ECHO_PIN = 7;
constexpr uint32_t SAMPLE_INTERVAL_MS = 500;
constexpr uint32_t ECHO_TIMEOUT_US = 30000;
uint32_t previousSampleMs = 0;

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, INPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  previousSampleMs = millis();
  Serial.println("[ULTRASONIC] TRIG=GPIO6 ECHO=GPIO7 ผ่าน divider 1k/2k");
}

void loop() {
  const uint32_t now = millis();
  if (now - previousSampleMs < SAMPLE_INTERVAL_MS) return;
  previousSampleMs = now;

  // ส่งพัลส์เริ่มวัด 10 us แล้วรอ ECHO โดยจำกัดเวลารอ
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  const unsigned long durationUs = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (durationUs == 0) {
    Serial.println("[ULTRASONIC] TIMEOUT: ไม่พบ ECHO ภายใน 30000 us");
    return;
  }
  // เวลาเสียงไป-กลับต้องหารสอง; ใช้ความเร็วเสียงประมาณ 0.0343 cm/us
  const float distanceCm = durationUs * 0.0343f / 2.0f;
  Serial.printf("[ULTRASONIC] echo=%lu us distance=%.1f cm\n", durationUs, distanceCm);
}

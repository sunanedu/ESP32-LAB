/*
 * Lab 8.4 : ตรวจเช็กอุปกรณ์ก่อนใช้งาน — ESP32-S3 + Relay 2CH (พินเดียวกับ Lab07)
 * วงจร: IN1 -> GPIO16, IN2 -> GPIO17, VCC -> แหล่งจ่ายโมดูล 5V
 * GND โมดูล/ขั้วลบแหล่งจ่าย/ESP32 ต้องร่วมกันตามวงจรโมดูล
 * ใช้โมดูล IN รองรับ 3.3V และไม่ดึง GPIO ขึ้น 5V; JD-VCC ต่อให้ตรงคู่มือ
 * เริ่มทดสอบโดยถอดโหลด; ถ้าต่อโหลดให้ใช้ DC แรงดันต่ำเท่านั้น ห้าม AC 220V
 * ไลบรารี: Arduino มากับ core ไม่ต้องติดตั้งเพิ่ม
 * Board: ESP32S3 Dev Module, Arduino-ESP32 core 3.x, Serial 115200
 */
#include <Arduino.h>

#if !defined(CONFIG_IDF_TARGET_ESP32S3)
#error "Select ESP32S3 Dev Module; this sketch uses the course ESP32-S3 pin map"
#endif

constexpr uint8_t RELAY_1_PIN = 16;
constexpr uint8_t RELAY_2_PIN = 17;
constexpr bool ACTIVE_LOW = true;  // ถ้ารุ่นจริง active HIGH ให้ปรับหลังตรวจคู่มือ
constexpr uint8_t ON_LEVEL = ACTIVE_LOW ? LOW : HIGH;
constexpr uint8_t OFF_LEVEL = ACTIVE_LOW ? HIGH : LOW;
constexpr uint8_t RELAY_PINS[] = {RELAY_1_PIN, RELAY_2_PIN};
constexpr uint32_t STEP_INTERVAL_MS = 2000;
constexpr uint8_t STEP_COUNT = 4;
uint8_t stepIndex = 0;
uint32_t previousStepMs = 0;

void applyStep() {
  // ลำดับ: ปิดทั้งคู่ -> CH1 -> CH2 -> เปิดทั้งคู่ -> กลับปิดทั้งคู่
  const bool channel1On = stepIndex == 1 || stepIndex == 3;
  const bool channel2On = stepIndex == 2 || stepIndex == 3;
  digitalWrite(RELAY_1_PIN, OFF_LEVEL);
  digitalWrite(RELAY_2_PIN, OFF_LEVEL);
  if (channel1On) digitalWrite(RELAY_1_PIN, ON_LEVEL);
  if (channel2On) digitalWrite(RELAY_2_PIN, ON_LEVEL);
  Serial.printf("[RELAY] CH1=%s CH2=%s | IN1=GPIO16 IN2=GPIO17\n",
                channel1On ? "ON" : "OFF", channel2On ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  // เตรียม latch OFF หลังผูกเป็น GPIO; ไม่รับรองสถานะก่อน setup/ช่วง Reset
  for (uint8_t pin : RELAY_PINS) {
    pinMode(pin, INPUT);
    digitalWrite(pin, OFF_LEVEL);
    pinMode(pin, OUTPUT);
  }
  applyStep();
  previousStepMs = millis();
}

void loop() {
  const uint32_t now = millis();
  if (now - previousStepMs < STEP_INTERVAL_MS) return;
  previousStepMs = now;
  stepIndex = (stepIndex + 1) % STEP_COUNT;
  applyStep();
}

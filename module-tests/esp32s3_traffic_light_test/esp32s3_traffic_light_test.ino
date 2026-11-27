/*
 * ทดสอบแยกโมดูล: ESP32-S3 + ไฟสามสี 8 mm แดง/เหลือง/เขียว
 * วงจร: VCC -> 3V3, RD -> GPIO10, YL -> GPIO11, GY -> GPIO12
 * ใช้โมดูลขั้วบวกร่วมที่มี R จำกัดกระแสในตัว รองรับ 3.3V และกระแส GPIO
 * มี 4 พิน VCC/RD/YL/GY ไม่มี GND แยก; ห้ามต่อ VCC เป็น GND
 * LOW=เปิด/HIGH=ดับตาม baseline; ตรวจวงจรจริงก่อนเปลี่ยน ACTIVE_LOW
 * ไลบรารี: Arduino มากับ core ไม่ต้องติดตั้งเพิ่ม
 * Board: ESP32S3 Dev Module, Arduino-ESP32 core 3.x, Serial 115200
 */
#include <Arduino.h>

#if !defined(CONFIG_IDF_TARGET_ESP32S3)
#error "Select ESP32S3 Dev Module; this sketch uses the course ESP32-S3 pin map"
#endif

constexpr uint8_t RED_PIN = 10;
constexpr uint8_t YELLOW_PIN = 11;
constexpr uint8_t GREEN_PIN = 12;
constexpr bool ACTIVE_LOW = true;
constexpr uint8_t ON_LEVEL = ACTIVE_LOW ? LOW : HIGH;
constexpr uint8_t OFF_LEVEL = ACTIVE_LOW ? HIGH : LOW;
constexpr uint8_t COLOR_PINS[] = {RED_PIN, YELLOW_PIN, GREEN_PIN};
constexpr uint32_t STEP_INTERVAL_MS = 2000;
const char* const STEP_NAMES[] = {"OFF", "GREEN", "YELLOW", "RED", "ALL"};
constexpr uint8_t STEP_COUNT = sizeof(STEP_NAMES) / sizeof(STEP_NAMES[0]);
uint8_t stepIndex = 0;
uint32_t previousStepMs = 0;

void showStep() {
  // ปิดทุกช่องก่อนเลือกสีใหม่ ลดการเปิดค้างระหว่างเปลี่ยนสถานะ
  for (uint8_t pin : COLOR_PINS) digitalWrite(pin, OFF_LEVEL);
  if (stepIndex == 1 || stepIndex == 4) digitalWrite(GREEN_PIN, ON_LEVEL);
  if (stepIndex == 2 || stepIndex == 4) digitalWrite(YELLOW_PIN, ON_LEVEL);
  if (stepIndex == 3 || stepIndex == 4) digitalWrite(RED_PIN, ON_LEVEL);
  Serial.printf("[LIGHT] %s | RD=GPIO10 YL=GPIO11 GY=GPIO12\n", STEP_NAMES[stepIndex]);
}

void setup() {
  Serial.begin(115200);
  // ผูกเป็น GPIO ก่อนตั้ง latch OFF แล้วจึงเปิด OUTPUT ตาม API core 3.x
  // การสั่ง OFF นี้ไม่รับรองระดับขาขณะ Reset หรือก่อน setup
  for (uint8_t pin : COLOR_PINS) {
    pinMode(pin, INPUT);
    digitalWrite(pin, OFF_LEVEL);
    pinMode(pin, OUTPUT);
  }
  showStep();
  previousStepMs = millis();
}

void loop() {
  const uint32_t now = millis();
  if (now - previousStepMs < STEP_INTERVAL_MS) return;
  previousStepMs = now;
  stepIndex = (stepIndex + 1) % STEP_COUNT;
  showStep();
}

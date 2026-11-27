/*
 * Lab 04.3 : ทดสอบโมดูลปุ่ม 3 ขา
 * คำอธิบาย : ดูระดับ raw/stable ทุก 500 ms และนับขอบกดหลัง debounce 40 ms
 * วงจร     : ปุ่มโมดูล 3 ขา: GND->GND, VCC->3V3, OUT->GPIO18; ใช้โมดูลที่ OUT ไม่เกิน 3.3V
 * ไลบรารี  : ไม่ใช้ library ภายนอก
 * Board    : ESP32S3 Dev Module; เลือกพิน ESP32-WROOM-32 อัตโนมัติเมื่อใช้ target ESP32 (core 3.x)
 */
#include <Arduino.h>

constexpr uint8_t BUTTON_PIN = 18;
constexpr uint8_t BUTTON_PRESSED_LEVEL = LOW;  // ถ้ากดแล้วอ่าน HIGH ให้เปลี่ยนเป็น HIGH
constexpr uint32_t DEBOUNCE_MS = 40;
bool previousRawLevel = HIGH;
bool stableButtonLevel = HIGH;
uint32_t lastRawChangeMs = 0;
uint32_t pressCount = 0;

void initButton() {
  // โมดูลมีตัวต้านทาน pull-up/pull-down ของตัวเอง จึงใช้ INPUT
  pinMode(BUTTON_PIN, INPUT);
  previousRawLevel = digitalRead(BUTTON_PIN);
  stableButtonLevel = previousRawLevel;
  lastRawChangeMs = millis();
}

bool buttonPressed(uint32_t now) {
  const bool rawLevel = digitalRead(BUTTON_PIN);
  if (rawLevel != previousRawLevel) {
    previousRawLevel = rawLevel;
    lastRawChangeMs = now;  // raw เปลี่ยนต้องเริ่มนับช่วงนิ่งใหม่
  }
  if (rawLevel != stableButtonLevel && now - lastRawChangeMs >= DEBOUNCE_MS) {
    stableButtonLevel = rawLevel;
    // รับเฉพาะขอบเปลี่ยนเข้าสู่สถานะกด; กดค้างไม่เพิ่มจำนวน
    if (stableButtonLevel == BUTTON_PRESSED_LEVEL) {
      pressCount++;
      return true;
    }
  }
  return false;
}

constexpr uint32_t REPORT_INTERVAL_MS = 500;
uint32_t previousReportMs = 0;

void setup() {
  Serial.begin(115200);
  initButton();
  Serial.println("[BUTTON] ปล่อย/กดค้างเพื่อเทียบ raw; ตั้ง BUTTON_PRESSED_LEVEL ให้ตรงโมดูล");
  Serial.printf("[BUTTON] configured pressed level=%u\n", BUTTON_PRESSED_LEVEL);
}

void loop() {
  const uint32_t now = millis();
  if (buttonPressed(now)) {
    Serial.printf("[BUTTON] press count=%lu\n", static_cast<unsigned long>(pressCount));
  }
  if (now - previousReportMs >= REPORT_INTERVAL_MS) {
    previousReportMs = now;
    Serial.printf("[BUTTON] raw=%u stable=%u count=%lu\n",
                  previousRawLevel, stableButtonLevel, static_cast<unsigned long>(pressCount));
  }
}

/*
 * ทดสอบแยกโมดูล: ESP32-S3 + ปุ่มกด momentary 3 พิน
 * วงจร: GND -> GND, VCC -> 3V3, OUT -> GPIO18
 * โมดูลมี R bias ในตัว ใช้ INPUT ไม่เพิ่ม R; OUT ต้องไม่เกิน 3.3V
 * อ่าน raw ตอนปล่อย/กดแล้วตั้ง BUTTON_PRESSED_LEVEL ให้ตรงรุ่น
 * ไลบรารี: Arduino มากับ core ไม่ต้องติดตั้งเพิ่ม
 * Board: ESP32S3 Dev Module, Arduino-ESP32 core 3.x, Serial 115200
 */
#include <Arduino.h>

#if !defined(CONFIG_IDF_TARGET_ESP32S3)
#error "Select ESP32S3 Dev Module; this sketch uses the course ESP32-S3 pin map"
#endif

constexpr uint8_t BUTTON_PIN = 18;
constexpr uint8_t BUTTON_PRESSED_LEVEL = LOW;  // เปลี่ยนเป็น HIGH ถ้ากดแล้ว raw=1
constexpr uint32_t DEBOUNCE_MS = 40;
constexpr uint32_t REPORT_INTERVAL_MS = 500;
bool previousRawLevel = HIGH;
bool stableLevel = HIGH;
uint32_t lastRawChangeMs = 0;
uint32_t previousReportMs = 0;
uint32_t pressCount = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
  // เริ่มจากระดับขาจริง ไม่สร้างขอบกดปลอมจากค่าตั้งต้น
  previousRawLevel = digitalRead(BUTTON_PIN);
  stableLevel = previousRawLevel;
  lastRawChangeMs = millis();
  Serial.println("[BUTTON] OUT=GPIO18; เทียบ raw ตอนปล่อย/กด ก่อนตั้งระดับกด");
}

void loop() {
  const uint32_t now = millis();
  const bool rawLevel = digitalRead(BUTTON_PIN);
  if (rawLevel != previousRawLevel) {
    previousRawLevel = rawLevel;
    lastRawChangeMs = now;
  }
  // รับสถานะใหม่เมื่อ raw คงที่ 40 ms; กดค้างไม่เพิ่มจำนวนซ้ำ
  if (rawLevel != stableLevel && now - lastRawChangeMs >= DEBOUNCE_MS) {
    stableLevel = rawLevel;
    if (stableLevel == BUTTON_PRESSED_LEVEL) {
      pressCount++;
      Serial.printf("[BUTTON] PRESSED count=%lu\n", static_cast<unsigned long>(pressCount));
    } else {
      Serial.println("[BUTTON] RELEASED");
    }
  }
  if (now - previousReportMs >= REPORT_INTERVAL_MS) {
    previousReportMs = now;
    Serial.printf("[BUTTON] raw=%u stable=%u pressed=%u count=%lu\n",
                  rawLevel, stableLevel, stableLevel == BUTTON_PRESSED_LEVEL,
                  static_cast<unsigned long>(pressCount));
  }
}

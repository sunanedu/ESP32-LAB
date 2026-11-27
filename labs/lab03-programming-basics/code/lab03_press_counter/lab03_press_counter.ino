/*
 * Lab 03.4 : นับจำนวนกดหลัง debounce
 * คำอธิบาย : เพิ่มตัวนับครั้งละหนึ่งเมื่อยืนยันขอบกดใหม่
 * วงจร     : S3/WROOM: โมดูลปุ่ม GND->GND, VCC->3V3, OUT->GPIO18; ไม่จำเป็นต้องต่อ LED
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก
 * Board    : ESP32S3 Dev Module (หลัก) / ESP32 Dev Module (WROOM), core 3.x
 */
#include <Arduino.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t BUTTON_PIN = 18;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t BUTTON_PIN = 18;
#else
#error "Select ESP32S3 Dev Module or ESP32 Dev Module"
#endif
constexpr uint8_t BUTTON_PRESSED_LEVEL = LOW;  // ถ้ากดแล้ว raw=HIGH ให้เปลี่ยนเป็น HIGH
constexpr uint32_t DEBOUNCE_MS = 40;
bool lastRawLevel = HIGH;
bool stableLevel = HIGH;
uint32_t lastRawChangeMs = 0;
uint32_t pressCount = 0;

// ใช้กติกา debounce เดียวกับการทดลอง 3.3
bool pressedEdge(uint32_t now) {
  const bool rawLevel = digitalRead(BUTTON_PIN);
  if (rawLevel != lastRawLevel) {
    lastRawChangeMs = now;
    lastRawLevel = rawLevel;
  }
  if (now - lastRawChangeMs < DEBOUNCE_MS || rawLevel == stableLevel) {
    return false;
  }
  stableLevel = rawLevel;
  return stableLevel == BUTTON_PRESSED_LEVEL;
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);  // โมดูลมี R ในตัว ใช้ระดับ OUT ที่ไม่เกิน 3.3V
  // เริ่ม raw/stable จากขาจริง กดค้างขณะเปิดเครื่องไม่สร้างขอบกดปลอม
  lastRawLevel = digitalRead(BUTTON_PIN);
  stableLevel = lastRawLevel;
  lastRawChangeMs = millis();
  Serial.println("[COUNT] ready: count=0");
}

void loop() {
  if (pressedEdge(millis())) {
    pressCount++;
    Serial.printf("[COUNT] %lu\n", static_cast<unsigned long>(pressCount));
  }
}

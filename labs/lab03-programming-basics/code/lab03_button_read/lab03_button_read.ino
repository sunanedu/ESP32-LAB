/*
 * Lab 03.2 : อ่านปุ่มดิบและตรวจ edge
 * คำอธิบาย : LED ติดขณะกด รายงาน raw ทุก 500 ms และ edge ตอนเปลี่ยนระดับ; ยังไม่ debounce
 * วงจร     : S3: GPIO5 -> R220Ω -> LED -> GND; WROOM: GPIO16; ปุ่มโมดูล VCC3V3/GND/OUT18
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก
 * Board    : ESP32S3 Dev Module (หลัก) / ESP32 Dev Module (WROOM), core 3.x
 */
#include <Arduino.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t LED_PIN = 5;  // LED ภายนอกหลัก คงขานี้จาก Lab02
constexpr uint8_t BUTTON_PIN = 18;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t LED_PIN = 16;  // สำหรับ ESP32-WROOM-32 รุ่นเดิม
constexpr uint8_t BUTTON_PIN = 18;
#else
#error "Select ESP32S3 Dev Module or ESP32 Dev Module"
#endif
constexpr uint8_t BUTTON_PRESSED_LEVEL = LOW;  // ถ้ากดแล้ว raw=HIGH ให้เปลี่ยนเป็น HIGH
bool previousLevel = HIGH;
constexpr uint32_t REPORT_INTERVAL_MS = 500;
uint32_t previousReportMs = 0;

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);  // โมดูลมี R pull-up/pull-down ในตัว
  previousLevel = digitalRead(BUTTON_PIN);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.printf("[BUTTON] pin=%u LED=%u pressedLevel=%u; ตรวจ raw ตอนปล่อย/กด\n",
                BUTTON_PIN, LED_PIN, BUTTON_PRESSED_LEVEL);
}

void loop() {
  const bool currentLevel = digitalRead(BUTTON_PIN);
  const bool pressed = currentLevel == BUTTON_PRESSED_LEVEL;
  digitalWrite(LED_PIN, pressed ? HIGH : LOW);

  // ต้องมีค่าก่อนหน้าเพื่อแยก "เพิ่งกด" ออกจาก "ยังกดค้าง"
  if (previousLevel != BUTTON_PRESSED_LEVEL && currentLevel == BUTTON_PRESSED_LEVEL) {
    Serial.println("[BUTTON] pressed raw edge");
  } else if (previousLevel == BUTTON_PRESSED_LEVEL && currentLevel != BUTTON_PRESSED_LEVEL) {
    Serial.println("[BUTTON] released raw edge");
  }
  previousLevel = currentLevel;
  const uint32_t now = millis();
  if (now - previousReportMs >= REPORT_INTERVAL_MS) {
    previousReportMs = now;
    Serial.printf("[BUTTON] raw=%u pressed=%u\n", currentLevel, pressed);
  }
}

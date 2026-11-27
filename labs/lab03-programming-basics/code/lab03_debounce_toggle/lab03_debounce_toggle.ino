/*
 * Lab 03.3 : Debounce และ toggle
 * คำอธิบาย : ยืนยันค่าที่คงที่ 40 ms แล้วสลับ LED เฉพาะขอบกด
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
constexpr uint32_t DEBOUNCE_MS = 40;
bool lastRawLevel = HIGH;
bool stableLevel = HIGH;
bool ledOn = false;
uint32_t lastRawChangeMs = 0;

// คืน true เฉพาะรอบที่ยืนยันขอบกดใหม่ ไม่คืน true ซ้ำระหว่างกดค้าง
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
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("[BUTTON] debounce 40 ms พร้อมทำงาน");
}

void loop() {
  const uint32_t now = millis();
  if (pressedEdge(now)) {
    ledOn = !ledOn;
    digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
    Serial.printf("[BUTTON] LED=%s\n", ledOn ? "ON" : "OFF");
  }
}

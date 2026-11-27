/*
 * Lab 03.5 : Array, for และ switch
 * คำอธิบาย : กดปุ่มเลือกดับทั้งหมด/กระพริบพร้อมกัน/กระพริบสลับกัน
 * วงจร     : S3: LED+R220Ω ที่ GPIO2/5; WROOM: GPIO2/16; โมดูลปุ่ม VCC3V3/GND/OUT18
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก
 * Board    : ESP32S3 Dev Module (หลัก) / ESP32 Dev Module (WROOM), core 3.x
 */
#include <Arduino.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t LED_PIN = 5;  // LED ภายนอกหลัก คงขานี้จาก Lab02
constexpr uint8_t SECOND_LED_PIN = 2;  // LED ภายนอกดวงที่สอง ไม่ใช่ onboard S3
constexpr uint8_t BUTTON_PIN = 18;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t LED_PIN = 16;  // สำหรับ ESP32-WROOM-32 รุ่นเดิม
constexpr uint8_t SECOND_LED_PIN = 2;  // LED ภายนอก หรือ onboard ที่ยืนยัน active HIGH
constexpr uint8_t BUTTON_PIN = 18;
#else
#error "Select ESP32S3 Dev Module or ESP32 Dev Module"
#endif
constexpr uint8_t BUTTON_PRESSED_LEVEL = LOW;  // ถ้ากดแล้ว raw=HIGH ให้เปลี่ยนเป็น HIGH
constexpr uint8_t LED_PINS[] = {SECOND_LED_PIN, LED_PIN};
constexpr size_t LED_COUNT = sizeof(LED_PINS) / sizeof(LED_PINS[0]);
constexpr uint32_t DEBOUNCE_MS = 40;
constexpr uint32_t STEP_INTERVAL_MS = 250;
uint8_t mode = 0;
bool phaseOn = false;
bool lastRawLevel = HIGH;
bool stableLevel = HIGH;
uint32_t lastRawChangeMs = 0;
uint32_t previousStepMs = 0;

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
  // index เริ่ม 0 และหยุดก่อน LED_COUNT จึงไม่ออกนอก array
  for (size_t i = 0; i < LED_COUNT; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);
  }
  pinMode(BUTTON_PIN, INPUT);  // โมดูลมี R ในตัว ใช้ระดับ OUT ที่ไม่เกิน 3.3V
  // เริ่ม raw/stable จากขาจริง กดค้างขณะเปิดเครื่องไม่สร้างขอบกดปลอม
  lastRawLevel = digitalRead(BUTTON_PIN);
  stableLevel = lastRawLevel;
  lastRawChangeMs = millis();
  Serial.println("[PATTERN] mode=0 (all off)");
}

void loop() {
  const uint32_t now = millis();
  if (pressedEdge(now)) {
    mode = (mode + 1) % 3;
    Serial.printf("[PATTERN] mode=%u\n", static_cast<unsigned int>(mode));
  }
  if (now - previousStepMs < STEP_INTERVAL_MS) {
    return;
  }
  previousStepMs = now;
  phaseOn = !phaseOn;

  // เลือกเพียงหนึ่ง case; break ออกจาก switch ไม่ใช่ออกจาก loop()
  switch (mode) {
    case 0:
      for (size_t i = 0; i < LED_COUNT; i++) {
        digitalWrite(LED_PINS[i], LOW);
      }
      break;
    case 1:
      for (size_t i = 0; i < LED_COUNT; i++) {
        digitalWrite(LED_PINS[i], phaseOn ? HIGH : LOW);
      }
      break;
    case 2:
      digitalWrite(LED_PINS[0], phaseOn ? HIGH : LOW);
      digitalWrite(LED_PINS[1], phaseOn ? LOW : HIGH);
      break;
  }
}

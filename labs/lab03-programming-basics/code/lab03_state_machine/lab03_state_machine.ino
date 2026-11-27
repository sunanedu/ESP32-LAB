/*
 * Lab 03.6 : State machine แบบ non-blocking
 * คำอธิบาย : กดเพื่อเปิด LED 1 วินาที แล้วรอปล่อยก่อนรับรอบใหม่
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
constexpr uint32_t ON_INTERVAL_MS = 1000;
enum class State { READY, LED_ON, WAIT_RELEASE };
State state = State::READY;
uint32_t enteredStateMs = 0;
bool lastRawLevel = HIGH;
bool stableLevel = HIGH;
uint32_t lastRawChangeMs = 0;

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

// รวมการเปลี่ยน state บันทึกเวลา และ output ไว้ที่เดียว
void changeState(State next, uint32_t now) {
  state = next;
  enteredStateMs = now;
  digitalWrite(LED_PIN, state == State::LED_ON ? HIGH : LOW);
  switch (state) {
    case State::READY: Serial.println("[STATE] READY"); break;
    case State::LED_ON: Serial.println("[STATE] LED_ON"); break;
    case State::WAIT_RELEASE: Serial.println("[STATE] WAIT_RELEASE"); break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);  // โมดูลมี R ในตัว ใช้ระดับ OUT ที่ไม่เกิน 3.3V
  // เริ่ม raw/stable จากขาจริง กดค้างขณะเปิดเครื่องไม่สร้างขอบกดปลอม
  lastRawLevel = digitalRead(BUTTON_PIN);
  stableLevel = lastRawLevel;
  lastRawChangeMs = millis();
  pinMode(LED_PIN, OUTPUT);
  changeState(State::READY, millis());
}

void loop() {
  const uint32_t now = millis();
  // อัปเดตตัวกรองทุกรอบและทุก state แม้ยังไม่พร้อมรับขอบกดใหม่
  const bool newPress = pressedEdge(now);
  switch (state) {
    case State::READY:
      if (newPress) {
        changeState(State::LED_ON, now);
      }
      break;
    case State::LED_ON:
      if (now - enteredStateMs >= ON_INTERVAL_MS) {
        changeState(State::WAIT_RELEASE, now);
      }
      break;
    case State::WAIT_RELEASE:
      if (stableLevel != BUTTON_PRESSED_LEVEL) {
        changeState(State::READY, now);
      }
      break;
  }
}

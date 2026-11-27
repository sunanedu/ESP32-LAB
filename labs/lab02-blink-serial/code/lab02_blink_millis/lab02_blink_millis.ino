/*
 * Lab 02.4 : Blink แบบ non-blocking
 * คำอธิบาย : กระพริบ LED ด้วย millis() โดยไม่หยุด loop()
 * วงจร     : ESP32-S3 GPIO5 / WROOM-32 GPIO16 -> R 220Ω -> LED anode; cathode -> GND
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก
 * Board    : ESP32S3 Dev Module / ESP32 Dev Module (WROOM-32), core 3.x
 */
#include <Arduino.h>

// LED ภายนอก active HIGH; เลือกขาตามชิปโดยใช้สายเดิมตลอดแล็บ
#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t LED_PIN = 5;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t LED_PIN = 16;
#else
#error "Lab02 supports ESP32-S3 and ESP32; define a verified pin mapping first."
#endif
constexpr uint32_t BLINK_INTERVAL_MS = 500;
uint32_t previousBlinkMs = 0;
bool ledState = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("[BLINK] non-blocking พร้อมทำงาน");
}

void loop() {
  const uint32_t now = millis();
  // ตรวจเวลาที่ผ่านไป; เมื่อยังไม่ถึงกำหนด loop จะทำงานส่วนอื่นต่อได้
  if (now - previousBlinkMs >= BLINK_INTERVAL_MS) {
    previousBlinkMs = now;
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    Serial.printf("[BLINK] LED=%s uptime=%lu ms\n",
                  ledState ? "ON" : "OFF",
                  static_cast<unsigned long>(now));
  }

  // loop ยังหมุนต่อได้ จึงเพิ่มงานอื่นในส่วนนี้ได้ภายหลัง
}

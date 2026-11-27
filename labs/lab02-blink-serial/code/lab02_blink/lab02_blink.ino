/*
 * Lab 02.1 : Blink แบบ delay
 * คำอธิบาย : กระพริบ LED ภายนอก ทุก 500 ms
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

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  Serial.println("[BLINK] เริ่มกระพริบ LED");
}

void loop() {
  // สั่ง HIGH ค้างครึ่งวินาที แล้วสั่ง LOW ค้างครึ่งวินาที จึงครบหนึ่งรอบ
  digitalWrite(LED_PIN, HIGH);
  Serial.println("[BLINK] ON");
  delay(500);
  digitalWrite(LED_PIN, LOW);
  Serial.println("[BLINK] OFF");
  delay(500);
}

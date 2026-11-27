/*
 * Lab 02.5 : ควบคุม LED ผ่าน Serial
 * คำอธิบาย : รับคำสั่ง on, off, toggle และ status จาก Serial Monitor
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
bool ledState = false;

void applyLedState() {
  // แปลงสถานะในตัวแปรเป็นระดับที่ขาจริง แล้วรายงานกลับ
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  Serial.printf("[CMD] LED=%s\n", ledState ? "ON" : "OFF");
}

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(200);
  pinMode(LED_PIN, OUTPUT);
  applyLedState();
  Serial.println("[CMD] พิมพ์ on, off, toggle หรือ status แล้วกด Enter");
}

void loop() {
  if (!Serial.available()) {
    return;
  }

  // อ่านข้อความจนพบ newline หรือรออักขระจน timeout; ช่วงนี้ยัง block ได้
  String command = Serial.readStringUntil('\n');
  command.trim();
  command.toLowerCase();

  if (command == "on") {
    ledState = true;
    applyLedState();
  } else if (command == "off") {
    ledState = false;
    applyLedState();
  } else if (command == "toggle") {
    ledState = !ledState;
    applyLedState();
  } else if (command == "status") {
    applyLedState();
  } else {
    Serial.printf("[CMD] ไม่รู้จักคำสั่ง: %s\n", command.c_str());
  }
}

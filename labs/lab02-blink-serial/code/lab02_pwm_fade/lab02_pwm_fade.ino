/*
 * Lab 02.6 : PWM Fade
 * คำอธิบาย : เพิ่ม/ลดความสว่าง LED ด้วย LEDC API ของ core 3.x
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
constexpr uint32_t PWM_FREQUENCY_HZ = 5000;
constexpr uint8_t PWM_RESOLUTION_BITS = 8;
constexpr uint16_t PWM_MAX = 255;
int duty = 0;
int step = 5;

void setup() {
  Serial.begin(115200);
  // core 3.x กำหนด PWM ด้วยหมายเลข pin และเลือก channel ให้อัตโนมัติ
  if (!ledcAttach(LED_PIN, PWM_FREQUENCY_HZ, PWM_RESOLUTION_BITS)) {
    Serial.println("[PWM] กำหนด LEDC ไม่สำเร็จ");
    while (true) {
      delay(1000);
    }
  }
  Serial.println("[PWM] เริ่ม fade 0–255");
}

void loop() {
  ledcWrite(LED_PIN, duty);
  // รูปแบบ label:value ใช้ดูตัวเลขใน Monitor และวาดกราฟใน Plotter ได้
  Serial.printf("duty:%d\n", duty);

  duty += step;
  // กลับทิศเมื่อถึงปลายช่วง 0 หรือ 255 ของความละเอียด 8-bit บน core 3.x
  if (duty >= PWM_MAX || duty <= 0) {
    duty = constrain(duty, 0, PWM_MAX);
    step = -step;
  }
  delay(30);
}

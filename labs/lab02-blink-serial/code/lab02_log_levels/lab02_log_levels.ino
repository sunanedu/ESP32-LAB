/*
 * Lab 02.3 : Console Log Levels
 * คำอธิบาย : แสดงข้อความระดับ Error, Warning และ Info
 * วงจร     : ESP32 ต่อคอมพิวเตอร์ด้วยสาย USB data
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก
 * Board    : ESP32S3 Dev Module / ESP32 Dev Module (WROOM-32), core 3.x, Core Debug Level = Info
 */
#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  delay(500);
  // ข้อความเหล่านี้เป็นการสาธิต ไม่ได้หมายความว่า hardware เกิดปัญหาจริง
  log_e("ตัวอย่าง Error: sensor ไม่ตอบสนอง");
  log_w("ตัวอย่าง Warning: ค่าใกล้ขีดจำกัด");
  log_i("ตัวอย่าง Info: ระบบเริ่มทำงานแล้ว");
}

void loop() {
  // ตั้ง Core Debug Level เป็น Info แล้ว compile/upload ใหม่เพื่อให้ log_i แสดง
  log_i("uptime = %lu ms", static_cast<unsigned long>(millis()));
  delay(3000);
}

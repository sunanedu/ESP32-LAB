/*
 * Lab 02.2 : Serial Hello
 * คำอธิบาย : ทดลอง print, println และ printf พร้อมค่าที่เปลี่ยนแปลง
 * วงจร     : ESP32 ต่อคอมพิวเตอร์ด้วยสาย USB data
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก
 * Board    : ESP32S3 Dev Module / ESP32 Dev Module (WROOM-32), core 3.x
 */
#include <Arduino.h>

uint32_t messageCount = 0;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("[SERIAL] ESP32 พร้อมทำงาน");
}

void loop() {
  // ตัวนับอยู่นอก loop จึงเก็บค่าต่อเนื่องข้ามรอบได้
  messageCount++;
  Serial.print("[SERIAL] ข้อความครั้งที่ ");
  Serial.println(messageCount);
  // %lu รับ unsigned long; cast ให้ชนิดตรงกันและใช้ \n ขึ้นบรรทัดใหม่
  Serial.printf("[SERIAL] เวลา = %lu ms, heap = %lu bytes\n",
                static_cast<unsigned long>(millis()),
                static_cast<unsigned long>(ESP.getFreeHeap()));
  delay(1000);
}

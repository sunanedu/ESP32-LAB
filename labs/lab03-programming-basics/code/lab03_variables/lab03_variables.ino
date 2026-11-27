/*
 * Lab 03.1 : ตัวแปรและฟังก์ชัน
 * คำอธิบาย : สร้างอุณหภูมิจำลองและแปลง Celsius เป็น Fahrenheit ทุก 2 วินาที
 * วงจร     : ESP32 ต่อคอมพิวเตอร์ด้วย USB data ไม่ต้องใช้ sensor
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก
 * Board    : ESP32S3 Dev Module; ESP32 Dev Module ใช้ได้ (core 3.x)
 */
#include <Arduino.h>

constexpr uint32_t REPORT_INTERVAL_MS = 2000;
uint32_t previousReportMs = 0;
uint32_t reportCount = 0;

// celsius เป็นพารามิเตอร์ local; return ส่งค่าคำนวณกลับผู้เรียก
float celsiusToFahrenheit(float celsius) {
  return celsius * 1.8F + 32.0F;
}

void setup() {
  Serial.begin(115200);
  Serial.println("[VAR] อุณหภูมิจำลอง ไม่ใช่ค่าจาก sensor");
}

void loop() {
  const uint32_t now = millis();
  if (now - previousReportMs < REPORT_INTERVAL_MS) {
    return;  // ยังไม่ถึงเวลา จบรอบนี้โดยไม่หยุดรอ
  }
  previousReportMs = now;
  reportCount++;

  // ค่าจำลองเปลี่ยนทีละ 0.1 C เพื่อให้เห็นความสัมพันธ์ของตัวแปร
  const float celsius = 25.0F + reportCount * 0.1F;
  const float fahrenheit = celsiusToFahrenheit(celsius);
  Serial.printf("[VAR] count=%lu C=%.1f F=%.1f\n",
                static_cast<unsigned long>(reportCount), celsius, fahrenheit);
}

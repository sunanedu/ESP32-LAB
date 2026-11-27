/*
 * Lab 04.1 : I2C Scanner สำหรับ OLED
 * คำอธิบาย : ตรวจ ACK ของ address 7-bit ช่วง 0x08-0x77 ทุก 3 วินาที
 * วงจร     : OLED I2C: VCC->3V3, GND->GND, SDA->GPIO8, SCL->GPIO9 บน S3 (ESP32 เดิม 21/22) (pull-up ฝั่ง 3.3V)
 * ไลบรารี  : Wire มากับ core; ไม่ใช้ library จอ
 * Board    : ESP32S3 Dev Module; เลือกพิน ESP32-WROOM-32 อัตโนมัติเมื่อใช้ target ESP32 (core 3.x)
 */
#include <Arduino.h>
#include <Wire.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t SDA_PIN = 8;
constexpr uint8_t SCL_PIN = 9;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t SDA_PIN = 21;
constexpr uint8_t SCL_PIN = 22;
#else
#error "Use ESP32-S3 or ESP32 with the verified course pin map"
#endif
constexpr uint32_t I2C_CLOCK_HZ = 100000;
constexpr uint32_t SCAN_INTERVAL_MS = 3000;
uint32_t previousScanMs = 0;
bool firstScan = true;
bool busReady = false;

void scanBus() {
  uint8_t found = 0;
  for (uint8_t address = 0x08; address <= 0x77; address++) {
    Wire.beginTransmission(address);
    const uint8_t error = Wire.endTransmission();
    if (error == 0) {
      Serial.printf("[I2C] ACK at 0x%02X\n", address);
      found++;
    }
  }
  Serial.printf("[I2C] total=%u\n", found);
  if (found == 0) Serial.println("[I2C] no ACK: ตรวจ 3V3/GND/SDA/SCL");
  // ACK บอกว่ามีอุปกรณ์ตอบ ไม่ยืนยันว่าเป็น SSD1306 หรือขนาดจอใด
}

void setup() {
  Serial.begin(115200);
  busReady = Wire.begin(SDA_PIN, SCL_PIN, I2C_CLOCK_HZ);
  Wire.setTimeOut(50);
  if (!busReady) Serial.println("[I2C] เริ่มบัสไม่สำเร็จ");
}

void loop() {
  if (!busReady) return;
  const uint32_t now = millis();
  if (firstScan || now - previousScanMs >= SCAN_INTERVAL_MS) {
    firstScan = false;
    previousScanMs = now;
    scanBus();
  }
}

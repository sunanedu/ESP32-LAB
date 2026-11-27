/*
 * Lab 07.1 : ทดสอบ OLED SSD1306
 * คำอธิบาย : ตรวจจอ I2C 128x64 address 0x3C และแสดงข้อความ/กรอบ
 * วงจร     : OLED VCC->3V3, GND->GND; ESP32 SDA/SCL=21/22, S3=8/9
 * ไลบรารี  : Adafruit GFX Library, Adafruit SSD1306
 * Board    : ESP32 Dev Module หรือ ESP32S3 Dev Module (Arduino-ESP32 core 3.x)
 */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr int OLED_RESET = -1;
constexpr uint8_t OLED_ADDRESS = 0x3C;
#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t I2C_SDA_PIN = 8;
constexpr uint8_t I2C_SCL_PIN = 9;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t I2C_SDA_PIN = 21;
constexpr uint8_t I2C_SCL_PIN = 22;
#else
#error "Lab07 supports ESP32 and ESP32-S3; define a verified pin mapping first."
#endif
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  // ACK ตรวจว่ามีอุปกรณ์ตอบที่ address; ยังไม่ยืนยันว่าเป็นรุ่น SSD1306
  Wire.beginTransmission(OLED_ADDRESS);
  if (Wire.endTransmission() != 0) {
    Serial.println("[OLED] no I2C ACK at 0x3C; check wiring/address");
    return;
  }
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println("[OLED] buffer/init failed");
    return;
  }

  // วาดข้อมูลลง buffer แล้วส่งขึ้นจอด้วย display()
  display.clearDisplay();
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2); display.setCursor(18, 12); display.println("ESP32");
  display.setTextSize(1); display.setCursor(18, 40); display.println("Smart Home Lab");
  display.display();
  Serial.println("[OLED] display ready");
}

void loop() {}

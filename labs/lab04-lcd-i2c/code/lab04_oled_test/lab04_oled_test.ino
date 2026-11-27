/*
 * Lab 04.2 : ทดสอบ OLED SSD1306
 * คำอธิบาย : แสดงข้อความและ uptime โดยวาด buffer ใหม่ทุกวินาที; ไม่มีจอยังรายงาน Serial
 * วงจร     : OLED I2C: VCC->3V3, GND->GND, SDA->GPIO8, SCL->GPIO9 บน S3 (ESP32 เดิม 21/22) (pull-up ฝั่ง 3.3V)
 * ไลบรารี  : Wire, Adafruit SSD1306, Adafruit GFX Library
 * Board    : ESP32S3 Dev Module; เลือกพิน ESP32-WROOM-32 อัตโนมัติเมื่อใช้ target ESP32 (core 3.x)
 */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t SDA_PIN = 8;
constexpr uint8_t SCL_PIN = 9;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t SDA_PIN = 21;
constexpr uint8_t SCL_PIN = 22;
#else
#error "Use ESP32-S3 or ESP32 with the verified course pin map"
#endif
constexpr uint8_t OLED_ADDRESS = 0x3C;  // เปลี่ยนเป็น 0x3D ตามผล I2C Scanner
constexpr uint32_t I2C_CLOCK_HZ = 100000;
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
static_assert(OLED_ADDRESS == 0x3C || OLED_ADDRESS == 0x3D,
              "Use the 7-bit OLED address 0x3C or 0x3D");

// ไม่ใช้ขา reset; กำหนด clock ระหว่างและหลังส่งภาพเป็น 100 kHz
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1,
                        I2C_CLOCK_HZ, I2C_CLOCK_HZ);
bool oledReady = false;

bool initOled() {
  if (!Wire.begin(SDA_PIN, SCL_PIN, I2C_CLOCK_HZ)) {
    Serial.println("[OLED] เริ่ม I2C ไม่สำเร็จ; ดูข้อมูลทาง Serial ต่อได้");
    return false;
  }
  Wire.setTimeOut(50);
  // ตรวจ ACK แยกจาก begin(): begin สำเร็จอย่างเดียวไม่ยืนยันว่าจอต่ออยู่
  Wire.beginTransmission(OLED_ADDRESS);
  const uint8_t error = Wire.endTransmission();
  if (error != 0) {
    Serial.printf("[OLED] no ACK at 0x%02X (error=%u); ตรวจไฟ/สาย/address\n",
                  OLED_ADDRESS, error);
    return false;
  }
  // ACK ยืนยันการตอบบนบัส ไม่ได้ระบุ driver; sketch นี้ใช้ SSD1306 128x64
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS, true, false)) {
    Serial.println("[OLED] init ไม่สำเร็จ; อาจจัดสรร buffer ไม่ได้");
    return false;
  }
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  Serial.printf("[OLED] ACK at 0x%02X; init=OK; ตรวจข้อความบนจอจริง\n",
                OLED_ADDRESS);
  return true;
}

constexpr uint32_t DISPLAY_INTERVAL_MS = 1000;
uint32_t previousDisplayMs = 0;
uint32_t frameCount = 0;
bool firstFrame = true;

void showFrame(uint32_t now) {
  if (!oledReady) return;
  // ล้าง buffer ทุกครั้ง ป้องกันตัวเลขใหม่ที่สั้นกว่าทิ้งเศษข้อความเดิม
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Lab04 OLED test");
  display.setTextSize(2);
  display.setCursor(0, 18);
  display.println("Hello!");
  display.setTextSize(1);
  display.setCursor(0, 44);
  display.print("Uptime: ");
  display.print(now / 1000);
  display.println(" s");
  display.print("Frame: ");
  display.print(frameCount);
  display.display();  // ส่ง buffer จาก RAM ไปยังจอจริง
}

void setup() {
  Serial.begin(115200);
  oledReady = initOled();
}

void loop() {
  const uint32_t now = millis();
  if (firstFrame || now - previousDisplayMs >= DISPLAY_INTERVAL_MS) {
    firstFrame = false;
    previousDisplayMs = now;
    frameCount++;
    showFrame(now);
    Serial.printf("[OLED] frame=%lu uptime=%lu s init=%s\n",
                  static_cast<unsigned long>(frameCount),
                  static_cast<unsigned long>(now / 1000), oledReady ? "OK" : "FAILED");
  }
}

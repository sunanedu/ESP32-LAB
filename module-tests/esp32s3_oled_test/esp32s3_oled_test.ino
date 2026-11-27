/*
 * ทดสอบแยกโมดูล: ESP32-S3 + OLED SH1106/SH1106G I2C 128x64
 * วงจร: OLED I2C VCC->3V3, GND->GND, SDA->GPIO8, SCL->GPIO9
 * ค่าเริ่มต้น SH1106 ตามจอผู้สอน; เปลี่ยน OLED_DRIVER ได้เมื่อใช้จอ SSD1306
 * ตรวจ ACK ที่ 0x3C/0x3D ไม่ได้ตรวจชนิด controller อัตโนมัติ
 * แสดงกรอบ/ข้อความคงที่และ uptime ทุกวินาที ไม่มีแถบหรือภาพเลื่อน
 * ไลบรารี: Adafruit SH110X, Adafruit GFX Library, Adafruit BusIO
 * ถ้าเลือก SSD1306 ให้ติดตั้ง Adafruit SSD1306 แทน Adafruit SH110X
 * Board: ESP32S3 Dev Module, Arduino-ESP32 core 3.x, Serial 115200
 */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>

// เลือก driver ให้ตรงชิปบนจอ ไม่สามารถแยกจาก address หรือสีของจอได้
#define OLED_DRIVER_SH1106 1
#define OLED_DRIVER_SSD1306 2
#ifndef OLED_DRIVER
#define OLED_DRIVER OLED_DRIVER_SH1106
#endif

#if OLED_DRIVER == OLED_DRIVER_SH1106
#include <Adafruit_SH110X.h>
#elif OLED_DRIVER == OLED_DRIVER_SSD1306
#include <Adafruit_SSD1306.h>
#else
#error "OLED_DRIVER must be OLED_DRIVER_SH1106 or OLED_DRIVER_SSD1306"
#endif

#if !defined(CONFIG_IDF_TARGET_ESP32S3)
#error "Select ESP32S3 Dev Module; this sketch uses the course ESP32-S3 pin map"
#endif

constexpr uint8_t SDA_PIN = 8;
constexpr uint8_t SCL_PIN = 9;
constexpr uint8_t OLED_ADDRESSES[] = {0x3C, 0x3D};
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr uint32_t I2C_CLOCK_HZ = 100000;
constexpr uint32_t UPDATE_MS = 1000;
// แต่ละ driver ใช้ framebuffer และคำสั่งส่งภาพของตนเอง
#if OLED_DRIVER == OLED_DRIVER_SH1106
constexpr char DRIVER_NAME[] = "SH1106G";
constexpr uint16_t PIXEL_ON = SH110X_WHITE;
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1,
                        I2C_CLOCK_HZ, I2C_CLOCK_HZ);
#else
constexpr char DRIVER_NAME[] = "SSD1306";
constexpr uint16_t PIXEL_ON = SSD1306_WHITE;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1,
                        I2C_CLOCK_HZ, I2C_CLOCK_HZ);
#endif
bool oledReady = false;
uint8_t oledAddress = 0;
uint32_t previousUpdateMs = 0;

bool initOled() {
  if (!Wire.begin(SDA_PIN, SCL_PIN, I2C_CLOCK_HZ)) {
    Serial.println("[OLED] I2C BEGIN FAILED");
    return false;
  }
  Wire.setTimeOut(50);
  // ใช้จอตัวเดียวและเลือก address แรกที่ตอบ ACK โดยเริ่มจาก 0x3C
  for (uint8_t address : OLED_ADDRESSES) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      oledAddress = address;
      break;
    }
  }
  if (oledAddress == 0) {
    Serial.println("[OLED] NO ACK at 0x3C/0x3D: ตรวจไฟ/SDA8/SCL9");
    return false;
  }
  // SH110X ใช้บัส Wire ที่เริ่มไว้; core 3.x คงพินเดิมเมื่อ begin ซ้ำ
#if OLED_DRIVER == OLED_DRIVER_SH1106
  if (!display.begin(oledAddress, true)) {
#else
  // SSD1306 มี periphBegin=false เพื่อไม่เริ่ม Wire ซ้ำ
  if (!display.begin(SSD1306_SWITCHCAPVCC, oledAddress, true, false)) {
#endif
    Serial.println("[OLED] BUFFER/INIT FAILED");
    return false;
  }
  display.setRotation(0);
  display.setTextColor(PIXEL_ON);
  display.setTextWrap(false);
  Serial.printf("[OLED] driver=%s ACK=0x%02X init=OK SDA=%u SCL=%u\n",
                DRIVER_NAME, oledAddress, SDA_PIN, SCL_PIN);
  Serial.println("[OLED] ตรวจกรอบครบ 4 ด้าน ข้อความอ่านได้ และ uptime เปลี่ยนบนจอจริง");
  return true;
}

void drawTestFrame(uint32_t now) {
  const unsigned long uptimeSeconds = now / 1000;
  // ตำแหน่งกรอบ/ข้อความคงที่ เปลี่ยนเฉพาะตัวเลขเพื่อแยกจากอาการภาพเลื่อน
  display.clearDisplay();
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, PIXEL_ON);
  display.setTextSize(1);
  display.setCursor(4, 4);
  display.print("OLED TEST");
  display.setCursor(4, 16);
  display.print(DRIVER_NAME);
  display.print(" 128x64");
  display.setCursor(4, 28);
  display.print("I2C: 0x");
  display.print(oledAddress, HEX);
  display.setCursor(4, 40);
  display.print("ABC 0123456789");
  display.setCursor(4, 52);
  display.print("Uptime: ");
  display.print(uptimeSeconds);
  display.print(" s");
  display.display();
  Serial.printf("[OLED] driver=%s uptime=%lu s; ตรวจภาพบนจอจริง\n",
                DRIVER_NAME, uptimeSeconds);
}

void setup() {
  Serial.begin(115200);
  oledReady = initOled();
  if (oledReady) {
    previousUpdateMs = millis();
    drawTestFrame(previousUpdateMs);
  }
}

void loop() {
  const uint32_t now = millis();
  if (!oledReady) {
    if (now - previousUpdateMs >= UPDATE_MS) {
      previousUpdateMs = now;
      Serial.println("[OLED] NOT READY: ตรวจสาย/รุ่นจอ แล้ว Reset เพื่อเริ่มใหม่");
    }
    return;
  }
  // อัปเดตทุกวินาทีโดยไม่ใช้คำสั่งเลื่อนภาพหรือแถบ DEMO
  if (now - previousUpdateMs >= UPDATE_MS) {
    previousUpdateMs = now;
    drawTestFrame(now);
  }
}

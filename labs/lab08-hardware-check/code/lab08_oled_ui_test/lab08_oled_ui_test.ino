/*
 * Lab 08.6 : ตรวจจอ OLED ด้วยข้อความและหน้า UI ที่เปลี่ยนค่า
 * วงจร: OLED I2C VCC->3V3, GND->GND, SDA->GPIO8, SCL->GPIO9
 * จอ baseline SSD1306 128x64; ตรวจ ACK ที่ 0x3C/0x3D ไม่ได้ระบุชนิด controller
 * แสดง OLED READY 3 วินาที แล้วแสดงชื่ออุปกรณ์/uptime/แถบ DEMO
 * ไลบรารี: Adafruit SSD1306, Adafruit GFX Library, Adafruit BusIO
 * Board: ESP32S3 Dev Module, Arduino-ESP32 core 3.x, Serial 115200
 */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#if !defined(CONFIG_IDF_TARGET_ESP32S3)
#error "Select ESP32S3 Dev Module; this sketch uses the course ESP32-S3 pin map"
#endif

constexpr uint8_t SDA_PIN = 8;
constexpr uint8_t SCL_PIN = 9;
constexpr uint8_t OLED_ADDRESSES[] = {0x3C, 0x3D};
constexpr int SCREEN_WIDTH = 128;
constexpr int SCREEN_HEIGHT = 64;
constexpr uint32_t I2C_CLOCK_HZ = 100000;
constexpr uint32_t INTRO_MS = 3000;
constexpr uint32_t UPDATE_MS = 1000;
constexpr char DEVICE_NAME[] = "S3_OLED";
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1,
                        I2C_CLOCK_HZ, I2C_CLOCK_HZ);
bool oledReady = false;
uint8_t oledAddress = 0;
uint32_t introStartedMs = 0;
uint32_t previousUpdateMs = 0;
bool firstUiFrame = true;

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
  // periphBegin=false เพื่อใช้บัสและพินที่กำหนดไว้ข้างต้น
  if (!display.begin(SSD1306_SWITCHCAPVCC, oledAddress, true, false)) {
    Serial.println("[OLED] BUFFER/INIT FAILED");
    return false;
  }
  display.setTextColor(SSD1306_WHITE);
  display.setTextWrap(false);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("OLED READY");
  display.println("SSD1306 128x64");
  display.print("Address: 0x");
  display.println(oledAddress, HEX);
  display.println("UI starts in 3 s");
  display.display();
  Serial.printf("[OLED] ACK=0x%02X init=OK; ตรวจข้อความบนจอจริง\n", oledAddress);
  return true;
}

void drawUi(uint32_t now) {
  const unsigned long uptimeSeconds = now / 1000;
  // DEMO เป็นความยาวแถบสำหรับทดสอบ UI ไม่ใช่ค่าเซ็นเซอร์ภายนอก
  const uint8_t demoPercent = uptimeSeconds % 101;
  display.clearDisplay();
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(4, 4);
  display.print("DEVICE CHECK");
  display.drawLine(1, 14, 126, 14, SSD1306_WHITE);
  display.setCursor(4, 18);
  display.print("Name: ");
  display.print(DEVICE_NAME);
  display.setTextSize(2);
  display.setCursor(4, 31);
  display.print(uptimeSeconds);
  display.print(" s");
  display.setTextSize(1);
  display.setCursor(4, 49);
  display.print("RUN  DEMO ");
  display.print(demoPercent);
  display.print('%');
  display.drawRect(4, 59, 120, 3, SSD1306_WHITE);
  display.fillRect(5, 60, static_cast<int>(demoPercent) * 118 / 100, 1, SSD1306_WHITE);
  display.display();
  Serial.printf("[OLED] name=%s uptime=%lu s demo=%u %%\n",
                DEVICE_NAME, uptimeSeconds, demoPercent);
}

void setup() {
  Serial.begin(115200);
  oledReady = initOled();
  introStartedMs = millis();
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
  // ใช้ timer แทน delay เพื่อเว้นข้อความแรกและอัปเดต UI เป็นรอบ
  if (now - introStartedMs < INTRO_MS) return;
  if (firstUiFrame || now - previousUpdateMs >= UPDATE_MS) {
    firstUiFrame = false;
    previousUpdateMs = now;
    drawUi(now);
  }
}

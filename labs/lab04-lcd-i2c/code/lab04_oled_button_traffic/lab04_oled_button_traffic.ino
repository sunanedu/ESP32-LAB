/*
 * Lab 04.6 : ปุ่มเลือกไฟและแสดงสถานะ OLED
 * คำอธิบาย : หนึ่งขอบกดวน OFF/RED/YELLOW/GREEN; OLED ไม่พร้อมยังทดสอบปุ่มและไฟได้
 * วงจร     : OLED I2C: VCC->3V3, GND->GND, SDA->GPIO8, SCL->GPIO9 บน S3 (ESP32 เดิม 21/22) (pull-up ฝั่ง 3.3V); ปุ่มโมดูล 3 ขา: GND->GND, VCC->3V3, OUT->GPIO18; ไฟจราจร 4 ขา common VCC->3V3, RD->GPIO10, YL->GPIO11, GY->GPIO12 (เขียว) บน S3 (ESP32 เดิม 17/23/32)
 * ไลบรารี  : Wire, Adafruit SSD1306, Adafruit GFX Library; ไฟต้องมี R จำกัดกระแสในตัว
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

constexpr uint8_t BUTTON_PIN = 18;
constexpr uint8_t BUTTON_PRESSED_LEVEL = LOW;  // ถ้ากดแล้วอ่าน HIGH ให้เปลี่ยนเป็น HIGH
constexpr uint32_t DEBOUNCE_MS = 40;
bool previousRawLevel = HIGH;
bool stableButtonLevel = HIGH;
uint32_t lastRawChangeMs = 0;
uint32_t pressCount = 0;

void initButton() {
  // โมดูลมีตัวต้านทาน pull-up/pull-down ของตัวเอง จึงใช้ INPUT
  pinMode(BUTTON_PIN, INPUT);
  previousRawLevel = digitalRead(BUTTON_PIN);
  stableButtonLevel = previousRawLevel;
  lastRawChangeMs = millis();
}

bool buttonPressed(uint32_t now) {
  const bool rawLevel = digitalRead(BUTTON_PIN);
  if (rawLevel != previousRawLevel) {
    previousRawLevel = rawLevel;
    lastRawChangeMs = now;  // raw เปลี่ยนต้องเริ่มนับช่วงนิ่งใหม่
  }
  if (rawLevel != stableButtonLevel && now - lastRawChangeMs >= DEBOUNCE_MS) {
    stableButtonLevel = rawLevel;
    // รับเฉพาะขอบเปลี่ยนเข้าสู่สถานะกด; กดค้างไม่เพิ่มจำนวน
    if (stableButtonLevel == BUTTON_PRESSED_LEVEL) {
      pressCount++;
      return true;
    }
  }
  return false;
}

#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t RED_PIN = 10;
constexpr uint8_t YELLOW_PIN = 11;
constexpr uint8_t GREEN_PIN = 12;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t RED_PIN = 17;
constexpr uint8_t YELLOW_PIN = 23;
constexpr uint8_t GREEN_PIN = 32;
#else
#error "Use ESP32-S3 or ESP32 with the verified course pin map"
#endif
constexpr bool TRAFFIC_ACTIVE_LOW = true;  // common VCC: LOW=ติด, HIGH=ดับ
constexpr uint8_t LED_ON_LEVEL = TRAFFIC_ACTIVE_LOW ? LOW : HIGH;
constexpr uint8_t LED_OFF_LEVEL = TRAFFIC_ACTIVE_LOW ? HIGH : LOW;
constexpr uint8_t TRAFFIC_PINS[] = {RED_PIN, YELLOW_PIN, GREEN_PIN};

void initTraffic() {
  // ผูกพินกับ GPIO ด้วย INPUT ก่อน จึงตั้ง latch OFF แล้วค่อยเปิด OUTPUT
  // ขั้นนี้เริ่มทำงานเมื่อเข้า setup แล้ว ไม่รับรองระดับขาขณะ reset/ก่อน setup
  for (uint8_t pin : TRAFFIC_PINS) {
    pinMode(pin, INPUT);
    digitalWrite(pin, LED_OFF_LEVEL);
    pinMode(pin, OUTPUT);
  }
}

void writeTraffic(bool redOn, bool yellowOn, bool greenOn) {
  digitalWrite(RED_PIN, redOn ? LED_ON_LEVEL : LED_OFF_LEVEL);
  digitalWrite(YELLOW_PIN, yellowOn ? LED_ON_LEVEL : LED_OFF_LEVEL);
  digitalWrite(GREEN_PIN, greenOn ? LED_ON_LEVEL : LED_OFF_LEVEL);
}

constexpr uint8_t STATE_COUNT = 4;
const char* const STATE_NAMES[] = {"OFF", "RED", "YELLOW", "GREEN"};
uint8_t trafficState = 0;
constexpr uint32_t REPORT_INTERVAL_MS = 500;
uint32_t previousReportMs = 0;

void drawTraffic() {
  if (!oledReady) return;
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Lab04 Traffic");
  display.setTextSize(2);
  display.setCursor(0, 20);
  display.println(STATE_NAMES[trafficState]);
  display.setTextSize(1);
  display.setCursor(0, 50);
  display.print("Presses: ");
  display.print(pressCount);
  display.display();
}

void applyTraffic() {
  writeTraffic(trafficState == 1, trafficState == 2, trafficState == 3);
  drawTraffic();
  Serial.printf("[SYSTEM] state=%s count=%lu oledInit=%s\n",
                STATE_NAMES[trafficState], static_cast<unsigned long>(pressCount),
                oledReady ? "OK" : "FAILED");
}

void setup() {
  Serial.begin(115200);
  initTraffic();  // ปิดไฟก่อนเริ่ม I2C หรือทำงานส่วนอื่น
  initButton();
  oledReady = initOled();
  applyTraffic();
}

void loop() {
  const uint32_t now = millis();
  if (buttonPressed(now)) {
    trafficState = (trafficState + 1) % STATE_COUNT;
    applyTraffic();
  }
  if (now - previousReportMs >= REPORT_INTERVAL_MS) {
    previousReportMs = now;
    Serial.printf("[SYSTEM] raw=%u stable=%u state=%s\n",
                  previousRawLevel, stableButtonLevel, STATE_NAMES[trafficState]);
  }
}

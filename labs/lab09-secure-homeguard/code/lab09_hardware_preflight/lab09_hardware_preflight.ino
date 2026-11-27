/*
 * Lab09.1 S3 HomeGuard: ตรวจวงจรและ UX รหัสจับคู่ท้องถิ่น
 * ESP32-S3: SH1106G SDA8/SCL9/VCC3V3/GND, DHT11 DATA4/VCC3V3/GND
 * Relay IN1=16/IN2=17/VCC5V/GNDร่วม; ปุ่ม OUT18/VCC3V3/GND
 * Libraries: Adafruit SH110X/GFX/BusIO, DHT sensor library/Unified Sensor
 * กดค้าง 2s เปิดหน้าต่าง DEMO 120s; รหัสนี้ยังไม่ใช่ SRP credential/root key
 * ไม่เปิด BLE/SoftAP/MQTT และคง Relay OFF ไม่มีการสั่งโหลดจากเครือข่าย
 * Arduino-ESP32 core3.x; Serial115200; CH343: USB CDC On Boot Disabled
 */
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <DHT.h>
#include <esp_random.h>
#include <bootloader_random.h>
#include <mbedtls/platform_util.h>

#if !defined(CONFIG_IDF_TARGET_ESP32S3)
#error "Select ESP32S3 Dev Module"
#endif
constexpr uint8_t SDA_PIN = 8;
constexpr uint8_t SCL_PIN = 9;
constexpr uint8_t DHT_PIN = 4;
constexpr uint8_t RELAY1_PIN = 16;
constexpr uint8_t RELAY2_PIN = 17;
constexpr uint8_t BUTTON_PIN = 18;
constexpr uint8_t BUTTON_PRESSED_LEVEL = LOW;
constexpr bool RELAY_ACTIVE_LOW = true;
constexpr uint32_t WINDOW_MS = 120000;
Adafruit_SH1106G display(128, 64, &Wire, -1, 100000, 100000);
DHT dht(DHT_PIN, DHT11);
bool oledReady = false;
bool windowOpen = false;
bool buttonArmed = false;
bool holdHandled = false;
int rawButton = HIGH;
int stableButton = HIGH;
uint32_t rawChangedMs = 0;
uint32_t heldSinceMs = 0;
uint32_t openedMs = 0;
uint32_t previousUiMs = 0;
uint32_t previousSensorMs = 0;
char demoCode[33] = {};

void closeWindow() {
  windowOpen = false;
  mbedtls_platform_zeroize(demoCode, sizeof(demoCode));
  Serial.println("[PAIR DEMO] CLOSED; relays remain OFF");
}

void openWindow(uint32_t now) {
  uint8_t randomBytes[16];
  // สเก็ตช์นี้ไม่เปิด RF/ADC จึงเปิด entropy ชั่วคราวแล้วปิดก่อนใช้ peripheral อื่น
  bootloader_random_enable();
  esp_fill_random(randomBytes, sizeof(randomBytes));
  bootloader_random_disable();
  constexpr char HEX_DIGITS[] = "0123456789abcdef";
  for (size_t i = 0; i < sizeof(randomBytes); ++i) {
    demoCode[i * 2] = HEX_DIGITS[randomBytes[i] >> 4];
    demoCode[i * 2 + 1] = HEX_DIGITS[randomBytes[i] & 15];
  }
  demoCode[32] = '\0';
  mbedtls_platform_zeroize(randomBytes, sizeof(randomBytes));
  openedMs = now;
  windowOpen = true;
  Serial.println("[PAIR DEMO] LOCAL ONLY / 120s / no network endpoint");
  // ผู้สอนต้องการอ่านรหัสจาก Serial แทนกล้อง: เฉพาะ DEMO ที่กดปุ่มบนบอร์ด
  Serial.printf("[PAIR DEMO] code=%s\n", demoCode);
}

void drawStatus(uint32_t now) {
  if (!oledReady) return;
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("S3 HomeGuard");
  display.println("PREFLIGHT / NOT PAIRED");
  display.println("Relay1/2: OFF LOCKED");
  if (windowOpen) {
    char firstLine[17];
    memcpy(firstLine, demoCode, 16);
    firstLine[16] = '\0';
    display.printf("DEMO %lu s\n", static_cast<unsigned long>((WINDOW_MS - (now - openedMs)) / 1000));
    display.println(firstLine);
    display.println(demoCode + 16);
    display.println("NO BLE/AP IN THIS TEST");
  } else {
    display.println("Hold button 2 seconds");
    display.println("for local code demo");
  }
  display.display();
}

void setup() {
  const uint8_t offLevel = RELAY_ACTIVE_LOW ? HIGH : LOW;
  // กำหนด OFF ก่อน network และก่อนทดสอบอื่น ยังต้องตรวจ glitch ช่วง boot จริง
  digitalWrite(RELAY1_PIN, offLevel);
  digitalWrite(RELAY2_PIN, offLevel);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);  // โมดูลมี R bias ในตัว
  rawButton = stableButton = digitalRead(BUTTON_PIN);
  buttonArmed = stableButton != BUTTON_PRESSED_LEVEL;
  dht.begin();
  if (Wire.begin(SDA_PIN, SCL_PIN, 100000)) {
    Wire.setTimeOut(50);
    for (uint8_t address : {0x3C, 0x3D}) {
      Wire.beginTransmission(address);
      if (Wire.endTransmission() == 0 && display.begin(address, true)) {
        oledReady = true;
        break;
      }
    }
  }
  if (oledReady) {
    display.setTextColor(SH110X_WHITE);
    display.setTextWrap(false);
  }
  Serial.printf("[PREFLIGHT] SH1106G=%s Relay16/17=OFF Button18 raw=%d\n",
                oledReady ? "init OK; inspect screen" : "not ready", rawButton);
  Serial.println("[PREFLIGHT] Local demo only; BLE/SoftAP/MQTT not started");
  drawStatus(millis());
}

void loop() {
  const uint32_t now = millis();
  if (windowOpen && now - openedMs >= WINDOW_MS) closeWindow();
  const int reading = digitalRead(BUTTON_PIN);
  if (reading != rawButton) {
    rawButton = reading;
    rawChangedMs = now;
  }
  if (now - rawChangedMs >= 40 && stableButton != rawButton) {
    stableButton = rawButton;
    if (stableButton != BUTTON_PRESSED_LEVEL) {
      buttonArmed = true;
      holdHandled = false;
    } else {
      heldSinceMs = now;
      if (windowOpen) {
        closeWindow();
        holdHandled = true;
      }
    }
  }
  if (buttonArmed && stableButton == BUTTON_PRESSED_LEVEL && !holdHandled &&
      now - heldSinceMs >= 2000) {
    holdHandled = true;
    openWindow(now);
  }
  if (now - previousSensorMs >= 2000) {
    previousSensorMs = now;
    const float humidity = dht.readHumidity();
    const float temperature = dht.readTemperature();
    if (isnan(humidity) || isnan(temperature)) Serial.println("[DHT11] READ FAILED");
    else Serial.printf("[DHT11] %.1f C %.1f %%\n", temperature, humidity);
  }
  if (now - previousUiMs >= 500) {
    previousUiMs = now;
    drawStatus(now);
  }
  delay(1);
}

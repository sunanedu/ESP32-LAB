/*
 * Lab 04.4 : ทดสอบโมดูลไฟจราจร
 * คำอธิบาย : วน OFF/RED/YELLOW/GREEN/ALL ด้วย millis เพื่อทดสอบขั้วและแต่ละสี
 * วงจร     : ไฟจราจร 4 ขา common VCC->3V3, RD->GPIO10, YL->GPIO11, GY->GPIO12 (เขียว) บน S3 (ESP32 เดิม 17/23/32); ใช้โมดูล 3.3V ที่มี R จำกัดกระแสในตัวเท่านั้น
 * ไลบรารี  : ไม่ใช้ library ภายนอก
 * Board    : ESP32S3 Dev Module; เลือกพิน ESP32-WROOM-32 อัตโนมัติเมื่อใช้ target ESP32 (core 3.x)
 */
#include <Arduino.h>

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

constexpr uint32_t STATE_INTERVAL_MS = 1500;
constexpr uint8_t STATE_COUNT = 5;
const char* const STATE_NAMES[] = {"OFF", "RED", "YELLOW", "GREEN", "ALL"};
uint8_t trafficState = 0;
uint32_t previousStateMs = 0;

void applyTraffic() {
  writeTraffic(trafficState == 1 || trafficState == 4,
               trafficState == 2 || trafficState == 4,
               trafficState == 3 || trafficState == 4);
  Serial.printf("[TRAFFIC] state=%s activeLow=%u\n",
                STATE_NAMES[trafficState], TRAFFIC_ACTIVE_LOW);
}

void setup() {
  Serial.begin(115200);
  initTraffic();
  applyTraffic();  // เริ่มด้วยทุกสีดับ ไม่ใช้ delay
  previousStateMs = millis();
}

void loop() {
  const uint32_t now = millis();
  if (now - previousStateMs >= STATE_INTERVAL_MS) {
    previousStateMs = now;
    trafficState = (trafficState + 1) % STATE_COUNT;
    applyTraffic();
  }
}

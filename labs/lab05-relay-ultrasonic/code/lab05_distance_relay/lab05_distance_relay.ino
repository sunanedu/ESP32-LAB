/*
 * Lab 05.4 : ควบคุม Relay ตามระยะพร้อม hysteresis
 * คำอธิบาย : CH1 ON ต่ำกว่า 20 cm, CH2 ON ต่ำกว่า 10 cm และมีช่วงปล่อย 3 cm
 * วงจร     : S3: Relay IN1/IN2 -> GPIO16/17; HC-SR04 TRIG -> 6, ECHO -> divider -> 7; ESP32 เดิม Relay26/27 TRIG25/ECHO33
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก
 * Board    : ESP32S3 Dev Module หรือ ESP32 Dev Module (Arduino-ESP32 core 3.x)
 */
#include <Arduino.h>

#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t TRIG_PIN = 6;
constexpr uint8_t ECHO_PIN = 7;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t TRIG_PIN = 25;
constexpr uint8_t ECHO_PIN = 33;
#else
#error "Use ESP32-S3 or ESP32 with the verified course pin map"
#endif
#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t RELAY_1_PIN = 16;
constexpr uint8_t RELAY_2_PIN = 17;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t RELAY_1_PIN = 26;
constexpr uint8_t RELAY_2_PIN = 27;
#else
#error "Use ESP32-S3 or ESP32 with the verified course pin map"
#endif
constexpr uint8_t RELAY_ON = LOW, RELAY_OFF = HIGH;
constexpr float CH1_ON_CM = 20.0f, CH1_OFF_CM = 23.0f;
constexpr float CH2_ON_CM = 10.0f, CH2_OFF_CM = 13.0f;
constexpr unsigned long SAMPLE_MS = 200;
bool relay1On = false, relay2On = false;
unsigned long lastSample = 0;
constexpr uint8_t MAX_INVALID_SAMPLES = 3;
uint8_t invalidSamples = 0;

float readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  const unsigned long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  return duration == 0 ? NAN : duration * 0.0343f / 2.0f;
}

// เปิดเมื่อระยะต่ำกว่า onThreshold และปิดเมื่อสูงกว่า offThreshold
bool applyHysteresis(bool current, float distance, float onThreshold,
                     float offThreshold) {
  if (!current && distance < onThreshold) return true;
  if (current && distance > offThreshold) return false;
  return current;
}

void writeRelays() {
  digitalWrite(RELAY_1_PIN, relay1On ? RELAY_ON : RELAY_OFF);
  digitalWrite(RELAY_2_PIN, relay2On ? RELAY_ON : RELAY_OFF);
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIG_PIN, OUTPUT); pinMode(ECHO_PIN, INPUT);
  pinMode(RELAY_1_PIN, OUTPUT); pinMode(RELAY_2_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  writeRelays();
}

void loop() {
  const unsigned long now = millis();
  if (now - lastSample < SAMPLE_MS) return;
  lastSample = now;

  const float distance = readDistanceCm();
  if (isnan(distance)) {
    // ตัวอย่างโหลด LED: ผิดพลาดติดกัน 3 รอบให้ปิดทั้งสองช่อง
    if (invalidSamples < MAX_INVALID_SAMPLES) invalidSamples++;
    if (invalidSamples >= MAX_INVALID_SAMPLES) {
      relay1On = false;
      relay2On = false;
      writeRelays();
    }
    Serial.printf("[SENSOR] invalid=%u CH1=%s CH2=%s\n",
                  static_cast<unsigned int>(invalidSamples),
                  relay1On ? "ON" : "OFF", relay2On ? "ON" : "OFF");
    return;
  }
  invalidSamples = 0;
  relay1On = applyHysteresis(relay1On, distance, CH1_ON_CM, CH1_OFF_CM);
  relay2On = applyHysteresis(relay2On, distance, CH2_ON_CM, CH2_OFF_CM);
  writeRelays();
  Serial.printf("[SYSTEM] distance=%.1f cm CH1=%s CH2=%s\n", distance,
                relay1On ? "ON" : "OFF", relay2On ? "ON" : "OFF");
}

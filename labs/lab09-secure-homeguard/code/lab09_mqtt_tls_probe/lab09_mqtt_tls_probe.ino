/*
 * Lab09.3 ESP32-S3: ตรวจ Wi-Fi/time/TLS/MQTT broker ที่เปลี่ยน host/port/CA ได้
 * คง Relay16/17 OFF ไม่ publish/subscribe ไม่มีคำสั่งควบคุมหรือ application root key
 * Libraries: PubSubClient; WiFi/NetworkClientSecure มากับ Arduino-ESP32 core3.x
 * คัดลอก secrets.example.h เป็น secrets.h ใส่ Wi-Fi/CA ในเครื่อง; Serial115200
 */
#include <Arduino.h>
#include <WiFi.h>
#include <NetworkClientSecure.h>
#include <PubSubClient.h>
#include <esp_random.h>
#include <time.h>
#include "secrets.h"
#if !defined(CONFIG_IDF_TARGET_ESP32S3)
#error "Select ESP32S3 Dev Module"
#endif
constexpr uint8_t RELAY_1_PIN = 16;
constexpr uint8_t RELAY_2_PIN = 17;
constexpr bool RELAY_ACTIVE_LOW = true;
NetworkClientSecure tls;
PubSubClient mqtt(tls);
bool ready = false;
uint32_t nextAttemptMs = 0;
uint32_t backoffMs = 1000;
char clientId[64];

void setup() {
  const uint8_t offLevel = RELAY_ACTIVE_LOW ? HIGH : LOW;
  digitalWrite(RELAY_1_PIN, offLevel);
  digitalWrite(RELAY_2_PIN, offLevel);
  pinMode(RELAY_1_PIN, OUTPUT);
  pinMode(RELAY_2_PIN, OUTPUT);
  Serial.begin(115200);
  if (strncmp(WIFI_SSID, "REPLACE_", 8) == 0 ||
      strstr(MQTT_CA_PEM, "-----BEGIN CERTIFICATE-----") == nullptr) {
    Serial.println("[PROBE] CONFIG REQUIRED: Wi-Fi and trusted CA in local secrets.h");
    return;
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  const uint32_t wifiStarted = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStarted < 20000) delay(100);
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[PROBE] Wi-Fi timeout; fix config then Reset");
    return;
  }
  // เวลาใช้ตรวจ TLS certificate เท่านั้น ไม่ใช้แทน monotonic anti-replay ของ protocol
  configTime(0, 0, "pool.ntp.org", "time.google.com");
  const uint32_t timeStarted = millis();
  while (time(nullptr) < 1767225600 && millis() - timeStarted < 15000) delay(100);
  if (time(nullptr) < 1767225600) {
    Serial.println("[PROBE] CLOCK INVALID; stop, no insecure fallback");
    return;
  }
  tls.setCACert(MQTT_CA_PEM);
  tls.setHandshakeTimeout(10);
  mqtt.setServer(MQTT_HOST, MQTT_PORT);  // ใช้ hostname เพื่อให้ TLS ตรวจชื่อด้วย
  mqtt.setSocketTimeout(3);
  mqtt.setKeepAlive(60);
  const uint64_t chipId = ESP.getEfuseMac();  // client ID ไม่ใช่ secret หรือการ authenticate
  snprintf(clientId, sizeof(clientId), "hg-probe-%08lx%08lx-%08lx",
      static_cast<unsigned long>(chipId >> 32), static_cast<unsigned long>(chipId),
      static_cast<unsigned long>(esp_random()));
  ready = true;
  Serial.printf("[PROBE] host=%s port=%u TLS verification enabled\n", MQTT_HOST, MQTT_PORT);
}

void loop() {
  if (!ready) { delay(10); return; }
  if (WiFi.status() != WL_CONNECTED) {
    mqtt.disconnect();
    ready = false;
    Serial.println("[PROBE] network lost; Reset to repeat this diagnostic");
    return;
  }
  if (mqtt.connected()) { mqtt.loop(); delay(1); return; }
  const uint32_t now = millis();
  if (static_cast<int32_t>(now - nextAttemptMs) < 0) { delay(1); return; }
  // Probe ยอม block connect แบบมี timeout; runtime จริงต้องแยก network task จาก safety task
  const bool connected = strlen(MQTT_USERNAME) == 0
      ? mqtt.connect(clientId)
      : mqtt.connect(clientId, MQTT_USERNAME, MQTT_PASSWORD);
  if (connected) {
    backoffMs = 1000;
    Serial.println("[PROBE] TLS+MQTT CONNECTED; application identity NOT authenticated");
  } else {
    Serial.printf("[PROBE] CONNECT FAILED state=%d; no insecure retry\n", mqtt.state());
    nextAttemptMs = millis() + backoffMs + esp_random() % 501;
    backoffMs = min(backoffMs * 2, static_cast<uint32_t>(60000));
  }
}

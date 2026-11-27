/*
 * Lab 06.2 : MQTT heartbeat และ LWT
 * คำอธิบาย : ส่ง uptime ทุก 5 วินาที พร้อม retained online/offline
 * วงจร     : ใช้ ESP32 และสาย USB เท่านั้น
 * ไลบรารี  : WiFi, PubSubClient 2.8.0
 * Board    : ESP32S3 Dev Module / ESP32 Dev Module (WROOM-32), core 3.x
 */
#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#if __has_include("secrets.h")
#include "secrets.h"
#else
#include "secrets.example.h"
#endif

constexpr char MQTT_HOST[] = "broker.emqx.io";
constexpr uint16_t MQTT_PORT = 1883;
constexpr unsigned long WIFI_RETRY_MS = 10000;
constexpr unsigned long MQTT_RETRY_MS = 5000;
WiFiClient network;
PubSubClient mqtt(network);
String statusTopic;
unsigned long lastWiFiRetry = 0, lastMqttRetry = 0;
bool wifiReported = false;

// MAC 48 บิตช่วยแยก Client ID ของแต่ละบอร์ด แต่ไม่ใช่รหัสลับ
String makeClientId() {
  const uint64_t chip = ESP.getEfuseMac();
  char suffix[13];
  snprintf(suffix, sizeof(suffix), "%04X%08X",
           static_cast<unsigned int>(static_cast<uint16_t>(chip >> 32)),
           static_cast<unsigned int>(static_cast<uint32_t>(chip)));
  return String("esp32-") + STUDENT_ID + "-" + suffix;
}

// ไม่วนรอ Wi-Fi ให้ loop กลับไปทำงานอื่นระหว่างรอลองใหม่
void serviceWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    if (!wifiReported) {
      Serial.printf("[WIFI] IP=%s RSSI=%d dBm\n",
                    WiFi.localIP().toString().c_str(), WiFi.RSSI());
      wifiReported = true;
    }
    return;
  }
  wifiReported = false;
  if (millis() - lastWiFiRetry >= WIFI_RETRY_MS) {
    lastWiFiRetry = millis();
    WiFi.reconnect();
    Serial.println("[WIFI] retry");
  }
}

void beginNetwork() {
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  lastWiFiRetry = millis();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setSocketTimeout(2);  // จำกัดการรอ packet; DNS/TCP อาจยัง block ชั่วครู่
  mqtt.setKeepAlive(15);
  if (!mqtt.setBufferSize(256)) Serial.println("[MQTT] buffer allocation failed");
}

String heartbeatTopic;
constexpr unsigned long HEARTBEAT_MS = 5000;
unsigned long lastHeartbeat = 0;

// เรียกครั้งเดียวต่อรอบเวลา ไม่ใช้ while รอ broker กลับมา
void connectMqtt() {
  if (WiFi.status() != WL_CONNECTED || mqtt.connected()) return;
  if (millis() - lastMqttRetry < MQTT_RETRY_MS) return;
  lastMqttRetry = millis();
  const String id = makeClientId();
  Serial.printf("[MQTT] connecting as %s\n", id.c_str());
  if (mqtt.connect(id.c_str(), statusTopic.c_str(), 0, true, "offline")) {
    Serial.println("[MQTT] connected");
    if (!mqtt.publish(statusTopic.c_str(), "online", true)) {
      Serial.println("[MQTT] status publish failed");
    }
  } else {
    Serial.printf("[MQTT] failed state=%d\n", mqtt.state());
  }
}

void setup() {
  Serial.begin(115200);
  statusTopic = String("esp32lab/") + STUDENT_ID + "/status";
  heartbeatTopic = String("esp32lab/") + STUDENT_ID + "/heartbeat";
  beginNetwork();
}

void loop() {
  serviceWiFi();
  connectMqtt();
  mqtt.loop();  // รับ packet และดูแล keep-alive ทุกรอบ
  if (mqtt.connected() && millis() - lastHeartbeat >= HEARTBEAT_MS) {
    lastHeartbeat = millis();
    char payload[24];
    snprintf(payload, sizeof(payload), "%lu", millis() / 1000UL);
    const bool ok = mqtt.publish(heartbeatTopic.c_str(), payload);
    Serial.printf("[MQTT] heartbeat=%s s publish=%s\n", payload, ok ? "ok" : "failed");
  }
}

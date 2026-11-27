/*
 * Lab 06.3 : ควบคุม LED ผ่าน MQTT
 * คำอธิบาย : รับ on/off/toggle แล้วรายงาน GPIO state กลับ
 * วงจร     : ESP32-S3 GPIO5 / WROOM-32 GPIO16 -> R 220Ω -> LED anode; cathode -> GND
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

// LED ภายนอก active HIGH; เลือกขาตามชิปโดยใช้สายเดิมตลอดแล็บ
#if defined(CONFIG_IDF_TARGET_ESP32S3)
constexpr uint8_t LED_PIN = 5;
#elif defined(CONFIG_IDF_TARGET_ESP32)
constexpr uint8_t LED_PIN = 16;
#else
#error "Lab06 supports ESP32-S3 and ESP32; define a verified pin mapping first."
#endif
String commandTopic, stateTopic;
bool ledOn = false;

// สถานะนี้เป็นค่าที่สั่ง GPIO ไม่ใช่ผลวัดแสงหรือกระแส LED จริง
void applyLed() {
  digitalWrite(LED_PIN, ledOn ? HIGH : LOW);
  const bool ok = mqtt.publish(stateTopic.c_str(), ledOn ? "on" : "off", true);
  Serial.printf("[MQTT] LED=%s state-publish=%s\n",
                ledOn ? "on" : "off", ok ? "ok" : "failed");
}

// payload ไม่จำเป็นต้องมี null terminator จึงอ่านตาม length เสมอ
void onMessage(char* topic, byte* payload, unsigned int length) {
  if (String(topic) != commandTopic || length > 16) {
    Serial.println("[MQTT] ignored topic/length");
    return;
  }
  String message;
  for (unsigned int i = 0; i < length; ++i) message += static_cast<char>(payload[i]);
  message.trim();
  message.toLowerCase();
  Serial.printf("[MQTT] topic=%s payload=%s\n", topic, message.c_str());
  if (message == "on") ledOn = true;
  else if (message == "off") ledOn = false;
  else if (message == "toggle") ledOn = !ledOn;
  else {
    Serial.println("[MQTT] ignored command");
    return;
  }
  applyLed();
}

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
    // clean session ต้อง subscribe ใหม่หลัง reconnect ทุกครั้ง
    const bool ok = mqtt.subscribe(commandTopic.c_str());
    Serial.printf("[MQTT] subscribe=%s topic=%s\n", ok ? "ok" : "failed",
                  commandTopic.c_str());
    applyLed();
  } else {
    Serial.printf("[MQTT] failed state=%d\n", mqtt.state());
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  const String prefix = String("esp32lab/") + STUDENT_ID;
  statusTopic = prefix + "/status";
  commandTopic = prefix + "/cmd/led";
  stateTopic = prefix + "/state/led";
  beginNetwork();
  mqtt.setCallback(onMessage);
}

void loop() {
  serviceWiFi();
  connectMqtt();
  mqtt.loop();
}

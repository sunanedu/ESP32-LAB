/*
 * Lab 07.3 : Smart Home firmware
 * คำอธิบาย : รวม DHT11, OLED, Relay 2CH และ MQTT พร้อมตรวจ valid/stale
 * วงจร     : ESP32: Relay=26/27, I2C=21/22; S3: Relay=16/17, I2C=8/9; DHT=4
 * ไลบรารี  : WiFi, PubSubClient 2.8.0, ArduinoJson 7.x, DHT, Adafruit GFX/SSD1306
 * Board    : ESP32 Dev Module หรือ ESP32S3 Dev Module (Arduino-ESP32 core 3.x)
 */
#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"
#if __has_include("secrets.h")
#include "secrets.h"
#else
#include "secrets.example.h"
#endif

WiFiClient network;
PubSubClient mqtt(network);
DHT dht(DHT_PIN, DHT11);
Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

String relay1SetTopic, relay2SetTopic, relay1StateTopic, relay2StateTopic;
String sensorTopic, statusTopic;
bool relay1On = false, relay2On = false, displayReady = false;
bool sensorValid = false, hasValidReading = false, wifiReported = false;
float lastTemperature = NAN, lastHumidity = NAN;
unsigned long lastMqttRetry = 0, lastWiFiRetry = 0;
unsigned long lastSensorRead = 0, lastValidRead = 0, lastDisplay = 0, lastState = 0;

// Client ID แยกแต่ละบอร์ดด้วย MAC 48 บิต ไม่ใช้เป็นรหัสลับ
String makeClientId() {
  const uint64_t chip = ESP.getEfuseMac();
  char suffix[13];
  snprintf(suffix, sizeof(suffix), "%04X%08X",
           static_cast<unsigned int>(static_cast<uint16_t>(chip >> 32)),
           static_cast<unsigned int>(static_cast<uint32_t>(chip)));
  return String("home-") + STUDENT_ID + "-" + suffix;
}

// รายงานค่าที่สั่ง GPIO ไม่ใช่ sensor ตรวจหน้าสัมผัส Relay
void publishRelayStates() {
  if (!mqtt.connected()) return;
  const bool ok1 = mqtt.publish(relay1StateTopic.c_str(), relay1On ? "ON" : "OFF", true);
  const bool ok2 = mqtt.publish(relay2StateTopic.c_str(), relay2On ? "ON" : "OFF", true);
  Serial.printf("[STATE] CH1=%s CH2=%s publish=%s/%s\n",
                relay1On ? "ON" : "OFF", relay2On ? "ON" : "OFF",
                ok1 ? "ok" : "failed", ok2 ? "ok" : "failed");
}

void applyRelays(bool report) {
  digitalWrite(RELAY_1_PIN, relay1On ? RELAY_ON : RELAY_OFF);
  digitalWrite(RELAY_2_PIN, relay2On ? RELAY_ON : RELAY_OFF);
  if (report) publishRelayStates();
}

// callback ยอมรับเฉพาะ topic ของตนและคำสั่ง ON/OFF
void onMessage(char* topic, byte* payload, unsigned int length) {
  if (length > 8) {
    Serial.println("[MQTT] ignored payload length");
    return;
  }
  String message;
  for (unsigned int i = 0; i < length; ++i) message += static_cast<char>(payload[i]);
  message.trim();
  message.toUpperCase();
  if (message != "ON" && message != "OFF") {
    Serial.println("[MQTT] ignored command");
    return;
  }
  const String incoming(topic);
  if (incoming == relay1SetTopic) relay1On = message == "ON";
  else if (incoming == relay2SetTopic) relay2On = message == "ON";
  else return;
  applyRelays(true);
}

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

// เว้นรอบลองใหม่; connect ยังเป็น synchronous call อาจรอเครือข่ายชั่วครู่
void connectMqtt() {
  if (WiFi.status() != WL_CONNECTED || mqtt.connected()) return;
  if (millis() - lastMqttRetry < MQTT_RETRY_MS) return;
  lastMqttRetry = millis();
  const String id = makeClientId();
  if (mqtt.connect(id.c_str(), statusTopic.c_str(), 0, true, "offline")) {
    const bool online = mqtt.publish(statusTopic.c_str(), "online", true);
    const bool sub1 = mqtt.subscribe(relay1SetTopic.c_str());
    const bool sub2 = mqtt.subscribe(relay2SetTopic.c_str());
    Serial.printf("[MQTT] connected online=%s subscribe=%s/%s\n",
                  online ? "ok" : "failed", sub1 ? "ok" : "failed", sub2 ? "ok" : "failed");
    publishRelayStates();
    lastState = millis();
  } else {
    Serial.printf("[MQTT] failed state=%d\n", mqtt.state());
  }
}

// รอบผิดพลาดส่ง null + valid=false; OLED เก็บค่าดีเก่าแต่ระบุ STALE
void readAndPublishSensor() {
  const float humidity = dht.readHumidity();
  const float temperature = dht.readTemperature();
  sensorValid = !isnan(humidity) && !isnan(temperature);
  if (sensorValid) {
    lastHumidity = humidity;
    lastTemperature = temperature;
    lastValidRead = millis();
    hasValidReading = true;
  } else {
    Serial.println("[SENSOR] NaN; previous reading is stale");
  }

  JsonDocument doc;
  if (sensorValid) {
    doc["temp"] = temperature;
    doc["hum"] = humidity;
  } else {
    doc["temp"] = nullptr;
    doc["hum"] = nullptr;
  }
  doc["valid"] = sensorValid;
  doc["uptime"] = millis() / 1000UL;
  if (hasValidReading) doc["age_s"] = (millis() - lastValidRead) / 1000UL;
  else doc["age_s"] = nullptr;
  char payload[160];
  if (measureJson(doc) >= sizeof(payload)) {
    Serial.println("[SENSOR] JSON too large");
    return;
  }
  const size_t length = serializeJson(doc, payload, sizeof(payload));
  if (mqtt.connected()) {
    const bool ok = mqtt.publish(sensorTopic.c_str(),
                                reinterpret_cast<const uint8_t*>(payload), length, false);
    Serial.printf("[SENSOR] %s publish=%s\n", payload, ok ? "ok" : "failed");
  } else {
    Serial.printf("[SENSOR] %s publish=offline\n", payload);
  }
}

// งานจอใช้ค่าที่เก็บไว้ ไม่อ่าน DHT ซ้ำทุกครั้งที่วาด
void updateDisplay() {
  if (!displayReady) return;
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.printf("WiFi:%s MQTT:%s", WiFi.status() == WL_CONNECTED ? "OK" : "--",
                 mqtt.connected() ? "OK" : "--");
  display.setCursor(0, 14);
  if (!hasValidReading) display.print("Temp: --.- C");
  else display.printf("Temp: %.1f C", lastTemperature);
  display.setCursor(0, 26);
  if (!hasValidReading) display.print("Hum : --.- %");
  else display.printf("Hum : %.1f %%", lastHumidity);
  display.setCursor(0, 38);
  if (!hasValidReading) display.print("S:NO DATA");
  else {
    const unsigned long age = millis() - lastValidRead;
    const bool fresh = sensorValid && age < SENSOR_INTERVAL_MS * 2;
    display.printf("S:%s age:%lus", fresh ? "OK" : "STALE", age / 1000UL);
  }
  display.setCursor(0, 52);
  display.printf("R1:%s R2:%s", relay1On ? "ON" : "OFF", relay2On ? "ON" : "OFF");
  display.display();
}

void setup() {
  Serial.begin(115200);
  Serial.printf("[BOOT] %s DHT=%u Relay=%u/%u I2C=%u/%u\n", ESP.getChipModel(),
                DHT_PIN, RELAY_1_PIN, RELAY_2_PIN, I2C_SDA_PIN, I2C_SCL_PIN);
  // ปิด output ตั้งแต่ setup; วงจรต้องจัดการช่วงก่อน setup ด้วย
  pinMode(RELAY_1_PIN, OUTPUT);
  digitalWrite(RELAY_1_PIN, RELAY_OFF);
  pinMode(RELAY_2_PIN, OUTPUT);
  digitalWrite(RELAY_2_PIN, RELAY_OFF);
  applyRelays(false);

  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  dht.begin();
  Wire.beginTransmission(OLED_ADDRESS);
  if (Wire.endTransmission() == 0) {
    displayReady = display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS);
    Serial.println(displayReady ? "[OLED] ready" : "[OLED] buffer/init failed; continue");
  } else {
    Serial.println("[OLED] no I2C ACK; continue without display");
  }

  const String prefix = String("home/") + STUDENT_ID;
  relay1SetTopic = prefix + "/relay/1/set";
  relay2SetTopic = prefix + "/relay/2/set";
  relay1StateTopic = prefix + "/relay/1/state";
  relay2StateTopic = prefix + "/relay/2/state";
  sensorTopic = prefix + "/sensor";
  statusTopic = prefix + "/status";
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  lastWiFiRetry = millis();
  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMessage);
  mqtt.setSocketTimeout(2);
  mqtt.setKeepAlive(15);
  if (!mqtt.setBufferSize(256)) Serial.println("[MQTT] buffer allocation failed");
}

void loop() {
  serviceWiFi();
  connectMqtt();
  mqtt.loop();
  const unsigned long now = millis();
  if (now - lastSensorRead >= SENSOR_INTERVAL_MS) {
    lastSensorRead = now;
    readAndPublishSensor();
  }
  if (now - lastDisplay >= DISPLAY_INTERVAL_MS) {
    lastDisplay = now;
    updateDisplay();
  }
  // ส่ง state เป็นระยะเพิ่มจากตอนรับคำสั่ง/เชื่อมใหม่ เพราะ publish ใช้ QoS 0
  if (mqtt.connected() && now - lastState >= STATE_INTERVAL_MS) {
    lastState = now;
    publishRelayStates();
  }
}

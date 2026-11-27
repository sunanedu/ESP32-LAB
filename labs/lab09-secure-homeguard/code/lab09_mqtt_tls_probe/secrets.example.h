#pragma once
// คัดลอกเป็น secrets.h ในเครื่องเท่านั้น ไฟล์จริงถูก gitignore
constexpr char WIFI_SSID[] = "REPLACE_WIFI_SSID";
constexpr char WIFI_PASSWORD[] = "REPLACE_WIFI_PASSWORD";
constexpr char MQTT_HOST[] = "broker.emqx.io";
constexpr uint16_t MQTT_PORT = 8883;
constexpr char MQTT_USERNAME[] = "";  // public profile เท่านั้น; private ใช้บัญชีเฉพาะ client
constexpr char MQTT_PASSWORD[] = "";
// ดาวน์โหลด CA จากผู้ให้บริการ ตรวจแหล่งที่มา แล้ววาง PEM เต็ม ห้าม setInsecure
constexpr char MQTT_CA_PEM[] = R"PEM(REPLACE_WITH_TRUSTED_CA_PEM)PEM";

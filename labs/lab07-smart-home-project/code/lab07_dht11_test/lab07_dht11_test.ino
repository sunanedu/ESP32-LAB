/*
 * Lab 07.2 : ทดสอบ DHT11
 * คำอธิบาย : อ่านอุณหภูมิและความชื้นทุก 2 วินาที พร้อมตรวจค่า NaN
 * วงจร     : DHT11 DATA->GPIO4, VCC->3V3, GND->GND; sensor เปล่าใช้ pull-up 10kΩ
 * ไลบรารี  : DHT sensor library, Adafruit Unified Sensor
 * Board    : ESP32 Dev Module (Arduino-ESP32 core 3.x)
 */
#include <Arduino.h>
#include <DHT.h>

constexpr uint8_t DHT_PIN = 4;
constexpr unsigned long READ_INTERVAL_MS = 2000;
DHT dht(DHT_PIN, DHT11);
unsigned long lastRead = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  Serial.println("[SENSOR] DHT11 ready");
}

void loop() {
  const unsigned long now = millis();
  if (now - lastRead < READ_INTERVAL_MS) return;
  lastRead = now;

  // อ่านไม่ถี่กว่า 2 วินาที และทิ้งรอบที่เซนเซอร์คืนค่าไม่ถูกต้อง
  const float humidity = dht.readHumidity();
  const float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("[SENSOR] อ่าน DHT11 ไม่สำเร็จ (NaN)");
    return;
  }
  Serial.printf("[SENSOR] temperature=%.1f C humidity=%.1f %%\n",
                temperature, humidity);
}

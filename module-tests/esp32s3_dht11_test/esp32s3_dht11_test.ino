/*
 * ทดสอบแยกโมดูล: ESP32-S3 + DHT11 (พินเดียวกับ Lab07)
 * วงจร: VCC/+ -> 3V3, GND/- -> GND, DATA/S/OUT -> GPIO4
 * ใช้รุ่นรองรับ 3.3V; ถ้าไม่มี pull-up ให้ต่อ R10k ระหว่าง DATA กับ 3V3
 * DHT11 เปล่า 4 ขา: มองหน้าตะแกรง ขาชี้ลง 1=VCC 2=DATA 3=NC 4=GND
 * ไลบรารี: DHT sensor library และ Adafruit Unified Sensor
 * Board: ESP32S3 Dev Module, Arduino-ESP32 core 3.x, Serial 115200
 */
#include <Arduino.h>
#include <DHT.h>

#if !defined(CONFIG_IDF_TARGET_ESP32S3)
#error "Select ESP32S3 Dev Module; this sketch uses the course ESP32-S3 pin map"
#endif

constexpr uint8_t DHT_PIN = 4;
constexpr uint32_t READ_INTERVAL_MS = 2000;
DHT dht(DHT_PIN, DHT11);
uint32_t previousReadMs = 0;

void setup() {
  Serial.begin(115200);
  dht.begin();
  previousReadMs = millis();
  Serial.println("[DHT11] DATA=GPIO4; รออ่านทุก 2 วินาที");
}

void loop() {
  const uint32_t now = millis();
  if (now - previousReadMs < READ_INTERVAL_MS) return;
  previousReadMs = now;

  // อ่านทั้งสองค่าและตรวจความสำเร็จ ไม่แทนค่าที่อ่านไม่ได้ด้วยศูนย์
  const float humidity = dht.readHumidity();
  const float temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("[DHT11] READ FAILED: ตรวจ VCC/GND/DATA และ pull-up");
    return;
  }
  Serial.printf("[DHT11] temperature=%.1f C humidity=%.1f %%\n",
                temperature, humidity);
}

# ไลบรารีที่ใช้

- `jsmn.h`: [zserge/jsmn commit25647e692c7906b96ffd2b05ca54c097948e879c](https://github.com/zserge/jsmn/blob/25647e692c7906b96ffd2b05ca54c097948e879c/jsmn.h), MIT; คง copyright/license ใน header ใช้ bounded token parser ตรวจโครงสร้างก่อน cJSON
- network_provisioning, protocomm Security2 patch1, ESP-MQTT, mbedTLS, cJSON, FreeRTOS และ Preferences: รวมมากับ Arduino-ESP32 core3.3.13 (ESP-IDF5.5.6+) ใช้ตาม license ของ upstream
- Adafruit SH110X2.1.15/GFX2.0.0/BusIO1.17.4 และ DHT sensor library1.4.7: ติดตั้งผ่าน Arduino Library Manager ตาม license ของ Adafruit

`secrets.h` และ binary ที่ฝัง QA Wi-Fi เป็นข้อมูลเฉพาะเครื่อง ห้ามเผยแพร่หรือ commit; source ใช้ secrets.example.h ที่เว้นค่ารหัสไว้

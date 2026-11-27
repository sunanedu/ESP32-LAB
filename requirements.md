# Requirements — ESP32 IoT Lab Series

> ฉบับเผยแพร่สำหรับผู้เรียน: ประวัติการพัฒนาและโครงสร้างแอปอ้างอิงด้านล่างเป็นหลักฐาน/เกณฑ์สำหรับสร้างเอง source สำเร็จเก็บเฉพาะผู้สอน ดู [นโยบายเผยแพร่](PUBLISHING.md)


## 1. ภาพรวมโครงการ
Repository บน GitHub สำหรับสอน ESP32 ตั้งแต่พื้นฐานจนถึงมินิโปรเจกต์ Smart Home
เนื้อหาเป็นภาษาไทย (ศัพท์เทคนิคใช้ภาษาอังกฤษ) ทุกแล็บอยู่ในไฟล์ `README.md` ของโฟลเดอร์แล็บ
โค้ดทุกชิ้นต้องคัดลอกไปใช้งานได้ทันที

## 2. กลุ่มผู้เรียนและสภาพแวดล้อม
- ผู้เรียน: นักเรียน/นักศึกษา/ผู้เริ่มต้น ไม่จำเป็นต้องมีพื้นฐาน C/C++
- OS: Windows 10 (64-bit)
- IDE: Arduino IDE 2.3.x ขึ้นไป
- Board core: `esp32 by Espressif Systems` เวอร์ชัน 3.x
- บอร์ดชุดผู้สอนสำหรับLab02–07: ESP32-S3 ตามLab07; รองรับESP32-WROOM-32เป็นชุดพินแยกตามtarget ห้ามใช้พินข้ามรุ่น
- MQTT Broker: `broker.emqx.io` (public)
- MQTT Client: Lab06 ใช้ MQTTX Web — https://mqttx.app/web-client; Lab07 ใช้ MQTTX Desktop เป็นมอนิเตอร์ 6 หัวข้อ
- Lab07 รองรับ ESP32-S3 เพิ่มจาก ESP32 รุ่นเดิม มีแอปอ้างอิง 1.0.1 บน Samsung S25 Ultra และขั้นตอนสร้าง standalone APK บน Windows

## 3. รายการแล็บ (Scope)
| Lab | หัวข้อ | อุปกรณ์หลัก |
|---|---|---|
| 01 | เริ่มต้นกับ ESP32 (ทฤษฎี + ติดตั้ง) | ESP32, สาย USB |
| 02 | หลักการเขียนโปรแกรม: Blink, Serial, Log | LED ภายนอก GPIO5 บน S3 |
| 03 | ตัวแปร เงื่อนไข ลูป ฟังก์ชัน | ปุ่มกด, LED |
| 04 | OLED I2C และโมดูลปุ่มกด/ไฟสามสี | OLED ประมาณ 1 นิ้ว, ปุ่ม GND/VCC/OUT, ไฟ VCC/RD/YL/GY |
| 05 | Relay 2CH และ Ultrasonic | Relay 2CH, HC-SR04 |
| 06 | WiFi และ MQTT | MQTTX Web |
| 07 | มินิโปรเจกต์ Smart Home + แอป React Native ผ่าน OpenCode | Android, SQLite ในโทรศัพท์, OLED, Relay 2CH, DHT11 |
| 08 | ตรวจเช็กอุปกรณ์ก่อนใช้งาน (แล็บพิเศษ) | DHT11, HC-SR04, ไฟสามสี, Relay2CH, ปุ่มโมดูล, OLED |
| 09 | S3 HomeGuard — Advanced Security IoT Smart Home | React Nativeต่อยอดLab07, ESP32-S3, SH1106G, BLE/SoftAP, brokerเปลี่ยนได้ |
| 10 | (สำรองไว้ขยายในอนาคต) | — |

## 4. Functional Requirements

### REQ-01 โครงสร้างแล็บมาตรฐาน
- ทุกแล็บต้องใช้ template จาก `assets/templates/lab-template.md`
- **Acceptance:** มีหัวข้อครบตามลำดับ: วัตถุประสงค์ → อุปกรณ์ → คำถามก่อนเรียน → ทฤษฎี → วงจร → การทดลอง → Troubleshooting → คำถามค้นคว้า → แบบทดสอบหลังเรียน → อ้างอิง

### REQ-02 คำถามก่อนเรียน (Pre-Lab)
- 5–8 ข้อ เป็นคำถามล้วน **ห้ามมีคำตอบ**
- แสดงในรูปแบบคลิกเพื่อเปิด (`<details><summary>`) โดยมีหัวข้อกำกับ
- **Acceptance:** ไม่มีคำว่า "เฉลย" หรือคำตอบในส่วนนี้

### REQ-03 คำถามค้นคว้าหลังเรียน
- 3–5 ข้อ เป็นโจทย์ปลายเปิดที่ต้องค้นข้อมูลเพิ่ม พร้อม "คำค้นแนะนำ"
- **Acceptance:** ทุกข้อมีคำค้นแนะนำอย่างน้อย 2 คำ

### REQ-04 แบบทดสอบหลังเรียน
- ปรนัย 4 ตัวเลือก (ก ข ค ง) จำนวน 10 ข้อ
- เรียงความยาก: ข้อ 1–3 ง่าย, ข้อ 4–7 ปานกลาง, ข้อ 8–10 ยาก
- เฉลยพร้อมคำอธิบายแยกไว้ที่ `answer-keys/labXX-answers.md`
- **Acceptance:** มีตัวเลือกถูกเพียง 1 ข้อ ตำแหน่งคำตอบกระจาย (ไม่ซ้ำตำแหน่งเดิมเกิน 3 ข้อติด)

### REQ-05 โค้ดพร้อมใช้งาน
- ทุก sketch อยู่ใน `labs/labXX-*/code/<sketch_name>/<sketch_name>.ino`
- มี comment ภาษาไทยอธิบายทุกบล็อกสำคัญ, ส่วนหัวไฟล์ระบุ: Lab, คำอธิบาย, การต่อวงจร, ไลบรารี
- ใช้ `Serial.begin(115200)`
- ค่า WiFi/MQTT แยกใน `secrets.h` และมี `secrets.example.h` (ไฟล์จริงอยู่ใน .gitignore)
- **Acceptance:** compile ผ่านบน core 3.x โดยไม่มี warning ร้ายแรง

- Lab04 คงพาธ `labs/lab04-lcd-i2c/` เพื่อรักษาลิงก์เดิมของผู้สอน; เนื้อหาและโค้ดปัจจุบันใช้ OLED และโมดูลปุ่ม/ไฟสามสีแทน LCD16×2
- Lab04 ใช้ baseline OLED SSD1306 I2C128×64/0x3C (เปลี่ยน address ตาม Scanner และตรวจ driver รุ่นจริง), โมดูลปุ่ม3พิน GND/VCC/OUT และไฟ3สี8mm VCC/RD/YL/GY; มีโค้ด Scanner, OLED, ปุ่ม, ไฟ, ปุ่ม+OLED และรวมสามโมดูลอย่างละชุด
- Lab04 วงจรหลัก ESP32-S3: SDA8/SCL9, OUT18, RD10/YL11/GY12; เลือกโมดูล3.3V ปุ่มมีRในตัวและไฟมีRจำกัดกระแสในตัวที่รับกระแสGPIOได้; commonVCCใช้activeLOWเป็นค่าเริ่มต้นที่ปรับได้ ไม่อ้างรุ่นจีนทุกโมดูลเหมือนกัน

- ยึดพินLab07 S3โดยไม่แก้Lab07: DHT4/OLED8,9/Relay16,17; Lab02–06ใช้LEDหลัก5/LEDตัวที่สอง2, ButtonOUT18, ไฟRD10/YL11/GY12, HC-SR04TRIG6/ECHO7ผ่านdivider; ทุกบทใช้พินเดิมและมีตารางVCC/GNDในassets/PINOUT.md

- ชุดทดสอบที่ผู้สอนขอแยกอยู่นอกบทเรียนใน `module-tests/<sketch>/<sketch>.ino`: DHT11, HC-SR04, ไฟRD/YL/GY, Relay2CH, ปุ่มGND/VCC/OUT และOLED รวม6โมดูล สำหรับESP32-S3 ใช้พินตามassets/PINOUT.md มีคู่มือและZIPรวม แต่ละไฟล์self-contained ไม่มีWiFi/MQTT/secrets และไม่เปลี่ยนLab07
- จอผู้สอนยืนยันเป็นSH1106/SH1106G: ไฟล์OLEDในmodule-testsใช้Adafruit_SH1106Gเป็นค่าเริ่มต้น128×64 และมีตัวเลือกSSD1306ตอนcompile; แสดงกรอบ/ข้อความกับuptimeโดยไม่มีแถบเคลื่อนไหว ตรวจACK0x3C/0x3Dแยกจากการระบุdriver มีคู่มือlibrary/เลือกdriverและระบุว่าตัวอย่างLab04/07/08เดิมยังใช้SSD1306

### REQ-06 อธิบายโค้ด
- ทุกโค้ดใน README ต้องมีตาราง "บรรทัด/ส่วน → ความหมาย" และผลลัพธ์ Serial Monitor ที่คาดหวัง

### REQ-07 รูปภาพ
- ทุกแล็บต้องมีอย่างน้อย: ภาพอุปกรณ์, แผนภาพการต่อวงจร, ภาพผลลัพธ์/หน้าจอ
- ลำดับการได้มาซึ่งภาพ: (1) สร้างเอง (Fritzing/Wokwi/draw.io/Mermaid) → (2) ภาพสัญญาอนุญาตเปิด (CC0/CC-BY/CC-BY-SA/Public Domain) → (3) ภาพมีลิขสิทธิ์ใช้ได้เฉพาะเป็นแนวทางใน `_reference/` (ไม่ commit) → (4) generate ด้วย AI
- **Acceptance:** ทุกภาพใน repo มีบันทึกใน `assets/IMAGE_SOURCES.md`

- Lab08 มีภาพต่อสายและตารางพินแยกครบ6โมดูล, ปุ่มดาวน์โหลดZIPรายโมดูล/รวม และโค้ดเต็มทดสอบพื้นฐานในREADME; OLEDมีUIชื่อ/uptimeเปลี่ยนค่า; โจทย์ต่อยอดDHT/ระยะ/ปุ่มtoggle/ลำดับไฟ/จัดUIมีแนวทางและจุดตรวจโดยไม่มีโค้ดเฉลย ส่วนRelayทดสอบเปิด–ปิดเท่านั้น

### REQ-08 MQTT
- Lab06 ใช้ topic prefix ไม่ซ้ำ: `esp32lab/<student_id>/...`; Lab07 ใช้ `home/<student_id>/...` ให้ตรง firmware และแอป
- ESP32: `broker.emqx.io:1883` (TCP)
- MQTTX Web: `wss://broker.emqx.io:8084/mqtt`
- Lab07 MQTTX Desktop: `mqtt://broker.emqx.io:1883`, MQTT3.1.1; ตั้ง alias/สีคงที่สำหรับ status, sensor, command และ reported state ใช้ STUDENT_ID ของตนให้ตรงทั้งสามฝั่ง
- **Acceptance:** ข้อความที่ส่งจาก MQTTX แสดงบน Serial Monitor ของ ESP32 และกลับกันได้

### REQ-09 ความปลอดภัย
- แล็บ Relay ห้ามใช้ไฟ AC 220V ในการทดลอง ให้ใช้โหลด DC แรงดันต่ำ (LED, พัดลม 5V)
- ระบุคำเตือนเรื่อง public broker: ห้ามส่งข้อมูลส่วนตัว

### REQ-10 Troubleshooting
- ทุกแล็บมีตาราง "อาการ → สาเหตุ → วิธีแก้" อย่างน้อย 5 รายการ

### REQ-11 มินิโปรเจกต์ React Native ด้วย OpenCode
- ผลงานสุดท้าย Lab07 คือแอป Android ที่ผู้เรียนสร้างด้วย React Native + Expo + TypeScript ผ่าน OpenCode
- เปิดแอปเข้า Dashboard ทันที มีหน้าเพิ่ม/แก้ไขอุปกรณ์/ตั้งค่า พื้นขาว การ์ดขอบคมบาง ไม่มีเงาฟุ้งรอบการ์ด/ปุ่ม และ icon font ตามข้อปรับปรุงจากการตรวจบนโทรศัพท์จริง
- เก็บ profiles/settings/history เป็นหลักบนโทรศัพท์ด้วย SQLite ไม่มี backend/cloud database ที่จำเป็นต่อการใช้งานหลัก
- แอปใช้ WSS `wss://broker.emqx.io:8084/mqtt`, ESP32 ใช้ TCP1883; MQTTX เป็น client สำหรับตรวจข้อความ ไม่ใช่ broker
- ชุดเริ่มต้น `app-blueprint/` มี 9 Markdown files (README, AGENTS, requirements, design, implementation, mqtt-contract, data-model, test-plan, opencode-prompts) และ assets ที่คัดลอกไปโปรเจกต์ใหม่ได้ครบ มีภาพจริง 4 หน้าของแอป 1.0.1 เป็นแบบหลักและ MQTTX template สำหรับ ID ของผู้เรียน ภาพ AI เดิมเก็บเป็นประวัติการออกแบบ
- **Acceptance ของชุดเอกสาร:** README แล็บสอนตั้งแต่กำหนด ID/package/board → เตรียม environment/เวอร์ชัน → OpenCode ทีละ Phase → firmware → MQTTX → build/install APK → ตรวจรับ พร้อมข้อจำกัดและผลที่ยังไม่ทดสอบ; ลิงก์/contract ตรง source, checklist ของผู้เรียนยังว่าง
- สเปกต้องกำหนด APP-01–12, FW-01–10 และ MX-01–06: UI ขอบ1dp ไม่มีเงา, SQLite schema2/retention, pending8s/presence15s/no replay, pin mapping แยก ESP32/S3 และ MQTTX 6 exact subscriptions/4สี ห้ามใช้ retained/cache เป็นหลักฐานว่าบอร์ดสดหรือ reported GPIO เป็นหลักฐานหน้าสัมผัส Relay
- ตามคำสั่งเพิ่มเติมของผู้สอน สร้างแอปอ้างอิงใน `app-blueprint/mobile/` พร้อมหลักฐานจาก source/lockfile/Android APK ของผู้สอน และหลักฐานจริง แยกจากชุดสเปกเริ่มต้นของผู้เรียน
- **Acceptance ของแอป:** ตรวจ APP-01–12/test-plan พร้อม Android/runtime/MQTT/storage evidence; ระบุผลที่ใช้ simulator และสิ่งที่ยังต้องตรวจบน ESP32/โทรศัพท์จริง ไม่ใช้ภาพ AI เป็นหลักฐานรัน

### REQ-12 Lab09 S3 HomeGuard

- ใช้Secure-IoT-Level-5.mdเป็นสเปกต้นทาง มีDECISIONSบันทึกข้อปรับ: publicEMQXเฉพาะlab, privateACLเป็นproductiongate, Arduino3.xสำหรับpreflight และESP-IDF APIsที่รวมในcore3.3.13สำหรับdevelopment runtime
- ตามคำสั่งผู้สอนล่าสุด READMEเป็นขั้นตอนปฏิบัติ/จุดตรวจ ลดทฤษฎี; รายละเอียดให้AIอยู่app-blueprint มีpromptทีละphaseและPLAN/PROGRESS/TEST_REPORTแยกสิ่งที่ทำแล้วกับงานruntimeที่ยังไม่ทำ
- มีhelperสร้างสำเนาsourceLab07 เปลี่ยนชื่อS3HomeGuard/package/iconและล็อกlegacycontrol, starterเลือกBLE/SoftAPและexportเฉพาะpublicbootstrapได้ ไม่แก้Lab07
- มีArduinoS3sketches hardwarepreflight(SH1106G/RelayOFF/DHT4/Button18), HKDF/AES-GCMselftest และTLSbrokerprobe พร้อมdownload/โค้ดเต็มแบบพับ; ไม่อ้างว่าpreflightเปิดprovisioningจริง
- runtimeเป้าหมายต้องจับคู่ผ่านBLEและSoftAPSecurity2ได้จริงพร้อมmanualcredentialจากOLED/Serialท้องถิ่น QRoptional, physicalwindow120s, nativeKeystore/AEAD/session/replay/expiry, brokertransactionทั้งสองฝั่ง ไม่มีonlinefirstpairing/เว็บกลางบังคับ
- ตามคำสั่งเพิ่มเติม สร้างแอปอ้างอิงจริงในapp-blueprint/mobile/พร้อมnativeKotlin/Keystore/Security2/TLSMQTT และstandaloneAPK, buildscript/lockfile/tests; ติดตั้งและทดสอบกับโทรศัพท์ผู้สอนเมื่อเชื่อมUSB
- เปิดหน้าหลักแบบเต็มจอ แสดงการ์ดรีเลย์และข้อมูลสภาพแวดล้อม ใช้ธีมขาว–น้ำเงินและภาพประกอบโทนฟ้าอ่อน เมนูเต็มขอบล่างใช้แบบ **Compact Center** จาก FloatingNavDemoV.2: ปุ่ม **เพิ่ม** วงกลม 50dp อยู่ตรงกลาง ไอคอนและข้อความของแท็บด้านข้างเปลี่ยนสีเมื่อเลือก ไม่มีเงาหรือพื้นไฮไลต์ตามแท็บ ปุ่มกลางเปลี่ยนสีพื้นเมื่อเลือก รวมการจัดการอุปกรณ์ โปรไฟล์ MQTT และความปลอดภัยไว้ใน **ตั้งค่า**
- เพิ่มcode/lab09_s3_homeguard/เป็นdevelopment runtime Arduino3.3.13ที่เรียกESP-IDF Security2patch1/ESP-MQTT/mbedTLSตรง ใช้custompartition16MBและพินเดิม บอร์ดเปล่าใช้BOOT0เฉพาะHG_BARE_BOARDและส่งsensorinvalid/null ห้ามอ้างGPIOเป็นผลหน้าสัมผัสRelay
- แยกBUILD/HOST/HARDWARE/NOTRUNตามผลจริง ไม่อ้างLevel5/productiongateครบ ไม่มีeFuse/billingหรือการทดสอบโหลดจริง; source/hardwarecredentials/Flashbackupต้องไม่commit

- ชุดพัฒนาLab09ต้องมีสารบัญแยกreference/starterและคู่มือBUILD_GUIDE/CONFIGURATION/USER_GUIDE/KNOWN_ISSUES, เพิ่มReleaseSRPself-testเป็นgateก่อนจับคู่, อ้างschema/contractตามsourceพร้อมdeviation และคงผลHOST/BUILD/PHONE/BOARD/NOTRUNแยกจากประวัติstarter

## 5. Non-Functional Requirements
- NFR-01 ภาษา: ไทยเป็นหลัก กระชับ อ่านง่าย
- NFR-02 Markdown แสดงผลถูกต้องบน GitHub (ตาราง, `<details>`, Mermaid)
- NFR-03 เวลาเรียนต่อแล็บ 2–3 ชั่วโมง; Lab07 เป็น capstone แบ่งช่วง firmware/เตรียมสเปกและช่วงสร้างแอปตามผู้สอน
- NFR-04 ทุกลิงก์ใช้งานได้ ไม่มีลิงก์เสีย
- NFR-05 (ทางเลือก) มีลิงก์จำลองบน Wokwi สำหรับผู้ที่ไม่มีบอร์ด

## 6. Out of Scope
- ESP-IDF, MicroPython, PlatformIO (กล่าวถึงได้เพียงสั้นๆ)
- งานไฟ AC แรงสูง
- การเผยแพร่แอป Android ขึ้น Play Store

# ESP32 IoT Lab Series

> **สร้างแอปด้วยตนเอง:** ชุดสาธารณะมีบทเรียน/สเปก/ภาพ แต่ไม่แจก source แอปสำเร็จ ดาวน์โหลดได้เฉพาะ [APK พื้นฐานสำหรับทดลอง](downloads/README.md) ไม่มี GitHub Releases หรือแท็กแจกซอร์สแอป

ชุดแล็บภาษาไทยสำหรับเรียนรู้ ESP32 ตั้งแต่การติดตั้งเครื่องมือ การเขียนโปรแกรมพื้นฐาน ไปจนถึงมินิโปรเจกต์ Smart Home เหมาะสำหรับผู้เริ่มต้นที่ใช้ **ESP32-S3 ตามชุดบอร์ดผู้สอน**, Windows 10/11 และ Arduino IDE 2.3.x ขึ้นไป

โค้ดในโครงการออกแบบสำหรับ **Arduino-ESP32 core 3.x** และแล็บทั่วไปใช้เวลาเรียนประมาณ 2–3 ชั่วโมง ส่วน Lab07 เป็นมินิโปรเจกต์ที่ผู้สอนแบ่งช่วงพัฒนาแอปเพิ่มเติม Lab08 ใช้ตรวจอุปกรณ์ และ Lab09 ต่อยอดด้านความปลอดภัย มีทั้งแอปอ้างอิง ชุดเริ่มต้น และคู่มือสร้าง/ตั้งค่า/ใช้งานพร้อมบทเรียนจากปัญหาที่พบ

## สารบัญแล็บ

Lab07 มี [แอป Android Preview1.0.1](https://github.com/sunanedu/ESP32-LAB/raw/refs/heads/main/downloads/esp32-smart-home-preview-1.0.1.apk) พร้อม [ภาพหน้าจอและสเปกสร้างเอง](labs/lab07-smart-home-project/app-blueprint/mobile/README.md) และ [ผลตรวจ Samsung S25 Ultra + ESP32-S3 จริง](labs/lab07-smart-home-project/app-blueprint/mobile/PHYSICAL_TEST_RESULTS.md) Lab02–07 ใช้พิน S3 คงที่ตาม Lab07 แล้ว ดู [ตารางต่อสายทั้งคอร์ส](assets/PINOUT.md); โค้ดยังเลือกพินรุ่น WROOM-32 แยกตาม target ได้

[บทเรียน Lab07 ฉบับสร้างตามระบบล่าสุด](labs/lab07-smart-home-project/README.md) สอนกำหนดสเปก → ใช้ OpenCode สร้างแอปขอบคม 4 หน้า → firmware → ตั้ง MQTTX แยกหัวข้อ/สี → build และติดตั้ง APK มี [ชุดเริ่มต้น 9 Markdown + assets](labs/lab07-smart-home-project/app-blueprint/README.md) และระบุข้อจำกัด/สิ่งที่ยังต้องทดสอบกับอุปกรณ์จริง

[ภาพผลลัพธ์สุดท้าย 27 พฤศจิกายน 2026](labs/lab07-smart-home-project/app-blueprint/evidence/final-2026-11-27/README.md): หน้าจอจริงจาก Samsung S25 Ultra ครบ4หน้า พร้อม MQTTX ที่แสดงข้อมูลสดจาก ESP32-S3 และรายละเอียดการจับภาพ

README หน้านี้เป็นจุดเริ่มต้นของคอร์ส คลิกชื่อบทเพื่อเข้า README ประจำแล็บ บทที่ขึ้นสถานะ **พร้อมเรียน** มีทฤษฎี ใบงาน ขั้นตอน โค้ดเต็ม คำอธิบาย ผลลัพธ์ คำถาม และแบบทดสอบครบในหน้าเดียว ส่วนบทสถานะ **Roadmap** แสดงขอบเขตที่จะพัฒนาใน Phase ถัดไป

| Lab | หัวข้อและเนื้อหาโดยสรุป | อุปกรณ์เด่น | สถานะ |
|---:|---|---|---|
| [01 — เริ่มต้นกับ ESP32](labs/lab01-esp32-introduction/) | รู้จักชิป/โมดูล/DevKit, สถาปัตยกรรม, pinout, ระบบไฟ, ติดตั้ง Arduino IDE และอ่านข้อมูลชิป | ESP32, สาย USB data | **พร้อมเรียน** |
| [02 — Blink, Serial และ Log](labs/lab02-blink-serial/) | โครงสร้าง sketch, Serial Monitor/Plotter, log level, `millis()`, Serial command และ PWM | LED ภายนอกGPIO5 | **พร้อมเรียน** |
| [03 — Programming Basics](labs/lab03-programming-basics/) | ตัวแปร เงื่อนไข ลูป ฟังก์ชัน, debounce, edge detection และ state machine | ปุ่มกด, LED | **พร้อมเรียน** |
| [04 — OLED และโมดูลปุ่ม/ไฟสามสี](labs/lab04-lcd-i2c/) | I²C scanner, OLED SSD1306, ทดสอบปุ่ม3พิน/ไฟ RD-YL-GY และรวมระบบ | OLED I2C, โมดูลปุ่ม, โมดูลไฟสามสี | **พร้อมเรียน** |
| [05 — Relay และ Ultrasonic](labs/lab05-relay-ultrasonic/) | Relay active LOW, HC-SR04, voltage divider, moving average และ hysteresis | Relay 2CH, HC-SR04 | **พร้อมเรียน** |
| [06 — WiFi และ MQTT](labs/lab06-wifi-mqtt/) | Wi-Fi STA, MQTT publish/subscribe, MQTTX Web, JSON telemetry, reconnect และ LWT | ESP32, MQTTX Web | **พร้อมเรียน** |
| [07 — Smart Home + React Native](labs/lab07-smart-home-project/) | รวม OLED/DHT11/Relay แล้วใช้ชุด Markdown ให้ OpenCode สร้างแอป Android พื้นขาว เก็บข้อมูลในโทรศัพท์ด้วย SQLite | Android, OLED, DHT11, Relay | **พร้อมเรียน — มีสเปกและแอป Android ตัวอย่าง** |
| [08 — ตรวจเช็กอุปกรณ์ก่อนใช้งาน](labs/lab08-hardware-check/) | แล็บพิเศษตรวจ6โมดูลทีละตัว ภาพต่อสาย ปุ่มดาวน์โหลด และโจทย์ต่อยอดพร้อมแนวทาง | DHT11, Ultrasonic, ไฟสามสี, Relay, ปุ่ม, OLED | **พร้อมเรียน — เปิดใช้ก่อนบทอื่นได้** |
| [09 — S3 HomeGuard](labs/lab09-secure-homeguard/README.md) | สร้าง Release APK/ตั้งค่า/ใช้งาน React Native + Native security, จับคู่ และตรวจปัญหา GATT/R8/lifecycle | ESP32-S3, SH1106G, DHT11, Relay, ปุ่ม, Android | **BLE/Session/GPIO บอร์ดเปล่าผ่าน; SoftAP/migration ยังรอตรวจครบ** |

[เริ่มจากแล็บ8เมื่อต้องการตรวจอุปกรณ์](labs/lab08-hardware-check/): ทำได้หลังเตรียมIDEตามLab01 แล้วกลับไปบทที่กำลังเรียน ไม่ต้องเรียนถึงLab07ก่อน

## โค้ดทดสอบแต่ละโมดูล

[Lab08 ฉบับเต็ม](labs/lab08-hardware-check/README.md) มี6โมดูลพร้อมOLED UI ภาพต่อสาย ปุ่มดาวน์โหลดรายชุด และ [ZIPรวม6โมดูล](labs/lab08-hardware-check/downloads/lab08-all-modules.zip) ส่วน[ชุดไฟล์ .ino แยก6โมดูล](module-tests/README.md) และ [ZIPชุด module-tests](module-tests/esp32s3-module-tests.zip) มี OLED รวมด้วยแล้ว ใช้พินคงที่ของคอร์สเหมือนกัน

จอผู้สอนยืนยันเป็น **SH1106/SH1106G**: ใช้ [ไฟล์ทดสอบ OLED ฉบับ SH1106](module-tests/esp32s3_oled_test/esp32s3_oled_test.ino) พร้อมคู่มือเลือก driver ในชุด module-tests ขณะนี้ตัวอย่างใน Lab04/07/08 ยังเป็น SSD1306

## อุปกรณ์รวมของคอร์ส

| อุปกรณ์ | จำนวนแนะนำ | ใช้ใน Lab |
|---|---:|---|
| ESP32-S3 + สาย USB data | 1 ชุด | 01–08 |
| Breadboard และสาย jumper | 1 ชุด | 02–08 |
| LED ภายนอก + ตัวต้านทาน 220 Ω | 2 ชุด | LEDหลัก5 ใน02/03/06 และLEDเสริม2 ใน03.5 |
| โมดูลปุ่มกด GND/VCC/OUT รองรับ3.3V มีRในตัว | 1 | 03–04, 08 |
| โมดูลไฟ8mmสามสี VCC/RD/YL/GY รองรับ3.3V มีRจำกัดกระแสในตัว | 1 | 04, 08 |
| Relay module 2 ช่อง | 1 | 05, 07, 08 |
| HC-SR04 + ตัวต้านทาน 1 kΩ/2 kΩ | 1 ชุด | 05, 08 |
| OLED SSD1306 I2C 128x64 ประมาณ1นิ้ว รองรับ3.3V | 1 | 04, 07, 08 ใช้ตัวเดียวร่วมกัน |
| DHT11 รองรับ 3.3V + R pull-up 10kΩ ถ้าใช้ sensor เปล่า | 1 | 07, 08 |
| แหล่งจ่าย/โหลด DC แรงดันต่ำ | ตามงาน | 05, 07, 08 |

> [!WARNING]
> แล็บนี้ไม่ใช้ไฟบ้าน AC 220V กับ Relay ให้ทดลองเฉพาะโหลด DC แรงดันต่ำเท่านั้น และห้ามป้อนแรงดัน 5V เข้าขา GPIO ของ ESP32

## Pin mapping คงที่ — ESP32-S3 สำหรับ Lab02–07

ต่ออุปกรณ์เดิมค้างที่พินเดิมทุกบท ยึด OLED/Relay/DHT11 จาก Lab07 และเว้นพินอื่นไม่ให้ชนกัน [ตารางครบทุกสาย รวม VCC/GND และวิธีต่อ ECHO divider](assets/PINOUT.md)

| อุปกรณ์ | ESP32-S3 GPIO | พินปลายทาง / ใช้ในแล็บ |
|---|---|---|
| LED ภายนอกตัวหลัก | 5 | ผ่าน R220Ω → A(+), K(−)→GND; 02/03/06 |
| LED ภายนอกตัวที่สอง | 2 | ผ่าน R220Ω → A(+), K(−)→GND; 03.5 ไม่ใช่LED onboardS3 |
| โมดูลปุ่ม | 18 | OUT; VCC3V3/GND; 03–04 ใช้ตัวเดิม |
| ไฟสามสี | 10 / 11 / 12 | RD / YL / GY; VCC3V3; 04 |
| OLED I2C | 8 / 9 | SDA / SCL; VCC3V3/GND; 04/เตรียม06/07 |
| Relay2CH | 16 / 17 | IN1 / IN2; VCC5V/GNDร่วม; 05/07 |
| HC-SR04 | 6 / 7 | TRIG / ECHOผ่านdivider1kΩ/2kΩ; VCC5V/GND; 05 |
| DHT11 | 4 | DATA; VCC3V3/GND; เตรียม06/07 |

บอร์ด CH343 ของผู้สอนเลือก ESP32S3 Dev Module และ USB CDC On Boot=Disabled ตามLab07 โค้ดเลือกชุดพินตาม target อัตโนมัติ ไม่แก้ค่าพินเองเมื่อเปลี่ยนบท GPIO19/20 สงวนทางUSB, GPIO26–37 หลีกเลี่ยงFlash/PSRAMของชุดS3นี้ ส่วนคำเตือนGPIO6–11ของWROOMไม่ใช่ข้อห้ามของS3 ดูตารางรุ่นเดิมใน [คู่มือพิน](assets/PINOUT.md)

## วิธีใช้งาน repository

1. เลื่อนขึ้นไปที่ส่วน **สารบัญแล็บ** และเลือกบทที่มีสถานะ **พร้อมเรียน**
2. เรียนเนื้อหาหลักให้จบใน README ของบท: วัตถุประสงค์ คำถามก่อนเรียน ทฤษฎี วงจร และการทดลอง
3. ใช้ code block ใน README เพื่ออ่านและทำความเข้าใจ แล้วคลิกลิงก์ไฟล์ `.ino` เมื่อต้องการเปิดด้วย Arduino IDE
4. ต่อวงจรตามภาพและตาราง pin mapping ก่อนจ่ายไฟ เลือก `ESP32S3 Dev Module` และ Port ของบอร์ดผู้สอน; USB CDC On Boot=Disabled สำหรับ CH343
5. อัปโหลดโค้ด เปิด Serial Monitor ที่ `115200 baud` แล้วเทียบผลลัพธ์ใน README
6. ทำภารกิจท้าทาย คำถามค้นคว้า และแบบทดสอบท้ายบท

เฉลยแบบทดสอบแยกไว้ใน `answer-keys/` สำหรับผู้สอนและเก็บเฉพาะเครื่อง โฟลเดอร์นี้ถูกระบุใน `.gitignore` จึงไม่ถูกอัปโหลดขึ้น GitHub

## โครงสร้างหลัก

```text
labs/labXX-*/README.md                 เนื้อหาแล็บ
labs/labXX-*/code/<sketch>/<sketch>.ino  โค้ดพร้อมใช้
labs/labXX-*/images/                  ภาพของแล็บ
answer-keys/                          เฉลยแบบทดสอบ (local only, ไม่ขึ้น GitHub)
assets/templates/                     แม่แบบแล็บ
assets/IMAGE_SOURCES.md               ที่มาและสัญญาอนุญาตของภาพ
```

## เอกสารอ้างอิงหลัก

- [Arduino-ESP32 documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/)
- [ESP32 Series Datasheet](https://documentation.espressif.com/esp32_datasheet_en.pdf)
- [Arduino IDE](https://www.arduino.cc/en/software)

## ผลตรวจคอร์ส

ผลตรวจ build/เอกสารทั้งคอร์สและขอบเขตที่ต้องทดสอบบน hardware อ่านได้ที่ [รายงาน QA](assets/QA_REPORT.md) ซึ่งระบุรุ่น core/library วิธีตรวจ และใบงานที่ใช้เก็บผลจริง

## License

โครงการนี้เผยแพร่ภายใต้ [GNU General Public License v3.0](LICENSE)

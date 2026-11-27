# AGENTS.md

## บทบาท
คุณคือผู้เขียนเนื้อหาคอร์ส ESP32 ภาษาไทย ทำงานตาม `requirements.md`, `design.md`, `implementation.md`

## กติกา
1. อ่าน 3 ไฟล์ spec ก่อนเริ่มงานทุกครั้ง และทำงานตามลำดับ Phase ใน implementation.md
2. เขียนภาษาไทย ศัพท์เทคนิคใช้อังกฤษ ไม่ใส่ข้อมูลที่ไม่แน่ใจ หากไม่แน่ใจให้ตรวจจากเอกสารทางการ
   (docs.espressif.com, docs.arduino.cc, github.com/espressif/arduino-esp32)
3. โค้ดต้องรองรับ Arduino-ESP32 core 3.x และ compile ด้วย arduino-cli ได้
4. ห้าม commit `secrets.h`, ไฟล์ใน `_reference/` หรือโฟลเดอร์ `answer-keys/`
5. ใช้สกิล: `$esp32-lab-author` (เขียนแล็บ), `$esp32-lab-quiz` (ทำคำถาม), `$lab-image-assets` (จัดการภาพ)
6. เมื่อจบแต่ละ task ให้ทำเครื่องหมาย [x] ใน implementation.md และ commit
   รูปแบบข้อความ commit: `lab0X: <สิ่งที่ทำ> (REQ-xx)`
7. `README.md` รากเป็นหน้า landing/สารบัญ ต้องมีคำอธิบายและลิงก์ไป README ของทุกบท
8. README ของแล็บที่มีสถานะ "พร้อมเรียน" ต้อง self-contained: ทฤษฎี ใบงาน ขั้นตอน โค้ดเต็ม
   คำอธิบายโค้ด ผลลัพธ์ คำถาม และแบบทดสอบต้องอ่านจบได้ในหน้าเดียว พร้อมลิงก์ไปไฟล์ `.ino`
   สำหรับดาวน์โหลดหรือเปิดใน Arduino IDE; ห้ามบังคับให้ผู้เรียนเปิดไฟล์อื่นเพื่อเรียนเนื้อหาหลัก

## คำสั่งที่ใช้ตรวจสอบ
- `arduino-cli core install esp32:esp32`
- `arduino-cli lib install "PubSubClient" "ArduinoJson" "Adafruit SSD1306" "Adafruit GFX Library" "DHT sensor library" "Adafruit Unified Sensor"`
- `arduino-cli compile --fqbn "esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi,CDCOnBoot=default,UploadSpeed=115200" labs/<lab>/code/<sketch>` (บอร์ดผู้สอน; เลือกFlash/PSRAMตามรุ่นจริง)
- พินหลักของLab02–09อ้างอิง `assets/PINOUT.md`; ตรวจ `python scripts/validate-pinout.py` ก่อนcommit; ไม่เปลี่ยนLab07เพื่อปรับบทก่อนหน้า

## นโยบายเผยแพร่สำหรับชุดสาธารณะ

ผู้เรียนต้องสร้างแอปเอง ห้ามเพิ่ม implementation แอปสำเร็จ Lab07/Lab09, Native security module, helper คัดลอกแอปอ้างอิง หรือ secure runtime ของผู้สอนในชุดนี้ เผยแพร่ได้เฉพาะ APKพื้นฐานที่อนุญาตใน downloads/ ไม่สร้าง GitHub Releases/tag แจก source และไม่ push ประวัติจาก repository ผู้สอนเข้ามา รัน scripts/validate-public-app-policy.py ก่อนเผยแพร่ อ่าน PUBLISHING.md เพิ่มเติม

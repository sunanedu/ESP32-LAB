---
name: esp32-lab-author
description: ใช้เมื่อต้องเขียนหรือแก้ไข README ของแล็บ ESP32 และโค้ด .ino ในโฟลเดอร์ labs/ ห้ามใช้กับงานทำแบบทดสอบหรืองานจัดการรูปภาพ
---

# ESP32 Lab Author

## ขั้นตอน
1. อ่านหัวข้อของแล็บใน `design.md` ส่วนที่ 4 และ pin mapping ในส่วนที่ 3
2. คัดลอก `assets/templates/lab-template.md` → `labs/labXX-*/README.md`
3. เขียนทฤษฎีเป็นย่อหน้าสั้น + ตาราง + แผนภาพ Mermaid (หลีกเลี่ยงย่อหน้ายาวเกิน 5 บรรทัด)
4. ทุกการทดลอง:
   - ขั้นตอนเป็นลำดับเลข
   - โค้ดเต็มใน code block `cpp` + บันทึกเป็นไฟล์ .ino ตาม path มาตรฐาน
   - ตาราง "ส่วนของโค้ด | หน้าที่"
   - ผลลัพธ์ Serial Monitor ที่คาดหวัง (code block `text`)
   - ภารกิจท้าทาย 1 ข้อ
5. Troubleshooting ≥5 แถว
6. เรียกใช้ `$esp32-lab-quiz` สำหรับส่วนคำถาม และ `$lab-image-assets` สำหรับภาพ
7. compile ทุก sketch ด้วย arduino-cli ถ้าไม่ผ่านต้องแก้ก่อนปิด task

## ข้อควรระวังทางเทคนิค
- PWM: ใช้ `ledcAttach(pin, freq, res)` และ `ledcWrite(pin, duty)` (API ของ core 3.x)
- ห้ามใช้ GPIO6–11; GPIO34–39 รับ input ได้อย่างเดียว; ADC2 ใช้ไม่ได้ขณะเปิด WiFi
- ขา ECHO ของ HC-SR04 ต้องผ่าน voltage divider
- MQTT: clientId ต้องไม่ซ้ำ (ต่อท้ายด้วย MAC address), เรียก `client.loop()` ทุกรอบ, มี reconnect แบบ non-blocking
- PubSubClient publish ได้แค่ QoS 0 — ต้องอธิบายข้อจำกัดนี้ในเนื้อหา

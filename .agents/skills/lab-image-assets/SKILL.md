---
name: lab-image-assets
description: ใช้เมื่อต้องหา ดาวน์โหลด สร้าง หรือ generate รูปภาพประกอบแล็บ และเมื่อต้องบันทึกแหล่งที่มาของภาพใน IMAGE_SOURCES.md
---

# Lab Image Assets

## ลำดับความสำคัญ
1. **สร้างเอง:** แผนภาพ → Mermaid ใน markdown; วงจร → Wokwi `diagram.json` หรือ Fritzing export PNG
2. **ภาพสัญญาอนุญาตเปิด:** Wikimedia Commons, Espressif/Adafruit/SparkFun (ตรวจสัญญาอนุญาตทุกครั้ง)
   - ดาวน์โหลด (PowerShell): `Invoke-WebRequest -Uri <url> -OutFile labs/labXX-*/images/<name>.png`
   - เพิ่มแถวใน `assets/IMAGE_SOURCES.md` (URL, ผู้สร้าง, สัญญาอนุญาต, วันที่)
   - ใส่ credit ใต้ภาพใน README
3. **ภาพมีลิขสิทธิ์:** ดาวน์โหลดเก็บไว้ที่ `_reference/labXX/` เท่านั้น เพื่อใช้เป็นแนวทาง
   **ห้ามคัดลอกเข้า labs/ และห้าม commit**
4. **Generate:** เมื่อไม่มีภาพที่ใช้ได้
   - เขียน prompt ลง `assets/prompts/labXX-<name>.md`
   - generate ด้วยเครื่องมือสร้างภาพที่มี (เช่น OpenAI Images API)
   - บันทึกเป็น `labXX-<name>-ai.png`, ใต้ภาพเขียนว่า "ภาพประกอบสร้างด้วย AI"
   - ถ้า generate ไม่ได้: สร้าง placeholder `![TODO: <คำอธิบาย>](images/placeholder.png)` และระบุใน implementation.md

## Prompt Template
```
Clean technical illustration, flat vector style, white background,
<subject e.g. "ESP32 DevKit V1 connected to a 2-channel relay module on a breadboard">,
labeled pins <list>, wires color-coded (red=5V, orange=3.3V, black=GND, other=signal),
no brand logos, high clarity, 16:9
```

## ภาพขั้นต่ำต่อแล็บ
| Lab | ภาพ |
|---|---|
| 01 | pinout, block diagram, ตระกูล ESP, Device Manager, Preferences, Boards Manager, Tools menu |
| 02 | วงจร LED, Serial Monitor, Serial Plotter |
| 03 | วงจรปุ่ม+LED, pull-up/pull-down, ภาพสัญญาณ bounce |
| 04 | โมดูล I2C หลัง LCD, วงจร, ผลลัพธ์บนจอ |
| 05 | ขั้ว relay, voltage divider, timing diagram ของ HC-SR04 |
| 06 | ภาพรวม MQTT, ฟอร์ม MQTTX Web, ภาพการ subscribe/publish |
| 07 | สถาปัตยกรรมระบบ, วงจรรวม, หน้าจอ OLED, หน้าจอแอป |

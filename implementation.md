# Implementation Plan — ESP32 IoT Lab Series

> ฉบับเผยแพร่สำหรับผู้เรียน: ประวัติการพัฒนาและโครงสร้างแอปอ้างอิงด้านล่างเป็นหลักฐาน/เกณฑ์สำหรับสร้างเอง source สำเร็จเก็บเฉพาะผู้สอน ดู [นโยบายเผยแพร่](PUBLISHING.md)


> Agent ทำทีละ Phase, ทำเครื่องหมาย [x] เมื่อเสร็จ, อ้างอิง REQ-xx ในข้อความ commit

## Phase 0 — Scaffold
- [x] 0.1 สร้างโครงสร้างโฟลเดอร์ตาม design.md
- [x] 0.2 `.gitignore`: `_reference/`, `**/secrets.h`, `.DS_Store`, `build/`
- [x] 0.3 `assets/templates/lab-template.md` (REQ-01)
- [x] 0.4 `assets/IMAGE_SOURCES.md` (ตารางว่าง) (REQ-07)
- [x] 0.5 `README.md` หลัก: แนะนำคอร์ส, สารบัญแล็บ, รายการอุปกรณ์รวม, ตาราง pin mapping, วิธีใช้งาน repo

## Phase 1 — Lab 01
- [x] 1.1 README ทฤษฎี: สถาปัตยกรรม, ตระกูล ESP, pinout, ระบบไฟ, ข้อจำกัด
- [x] 1.2 ขั้นตอนติดตั้งบน Windows 10 พร้อมภาพทุกขั้น
- [x] 1.3 `code/lab01_chip_info/lab01_chip_info.ino`
- [x] 1.4 ภาพ: pinout, block diagram (Mermaid), screenshot หรือภาพ generate ของหน้าจอ IDE
- [x] 1.5 Pre-lab 6 ข้อ, คำถามค้นคว้า 4 ข้อ, แบบทดสอบ 10 ข้อ + answer key

### ปรับปรุง Lab 01 — บทเรียนสำหรับผู้เริ่มต้น
- [x] 1.R1 เรียบเรียงการติดตั้ง: ลิงก์ดาวน์โหลด/Installer, USB data/COM, Preferences URL, core, Board/Port และจุดตรวจแต่ละช่วง
- [x] 1.R2 สอนจาก sketch เริ่มต้นทีละส่วน: setup/loop, Serial, preprocessor/include, ตัวแปร/ชนิดข้อมูล, printf, ฟังก์ชัน/array/error handling แล้วจบด้วย `.ino` เดียว
- [x] 1.R3 เพิ่มภาพ pinout พื้นขาวจากแหล่งเปิดพร้อมเครดิต และภาพหน้าจอจริง 3 ภาพจาก Arduino IDE; ปรับภาพจำลอง Device Manager ให้เป็น tree แบบ Windows
- [x] 1.R4 ตรวจ compile, โค้ด README ตรงไฟล์, template, quiz, แหล่งที่มาภาพและลิงก์ก่อน commit/push

ผลตรวจรอบปรับปรุง: `lab01_chip_info` compile ผ่านด้วย Arduino CLI 1.6.0 ที่ติดมากับ IDE, Arduino-ESP32 3.3.13, FQBN `esp32:esp32:esp32`, `--warnings all`; โค้ดเต็ม README ตรงกับ `.ino`; repository/quiz validators ผ่านครบ 7 บท และ `markdown-link-check` ของ Lab 01 ผ่าน

หมายเหตุการจับภาพ: เครื่องมือเปิด MMC ได้แต่ควบคุมหน้าต่างสิทธิ์สูงไม่ได้ และการตรวจ serial port ไม่พบ COM ในขณะตรวจ จึงใช้ภาพจำลอง Device Manager/Boards Manager/Tools/Serial Monitor ที่ระบุชัด ไม่อ้างว่าได้ upload หรืออ่านค่าจาก hardware จริงในรอบปรับปรุงนี้

## Phase 2 — Lab 02
- [x] 2.1 sketches: `lab02_blink`, `lab02_serial_hello`, `lab02_log_levels`,
      `lab02_blink_millis`, `lab02_serial_command`, `lab02_pwm_fade`
- [x] 2.2 ตารางอธิบายโค้ด + ผลลัพธ์ Serial ที่คาดหวังทุก sketch
- [x] 2.3 ภาพวงจร LED ภายนอก + ภาพหน้าจอ Serial Plotter
- [x] 2.4 คำถาม/แบบทดสอบ/เฉลย

### ปรับปรุง Lab 02 — เรียนผ่านการทดลองทีละขั้น
- [x] 2.R1 เพิ่มการเตรียม IDE/วงจร, LED ค้างก่อน Blink, print/println, log, millis, parser และ PWM พร้อมจุดตรวจ/ใบงาน
- [x] 2.R2 โค้ดเต็มอ่านง่ายตรงกับ `.ino` ทั้ง 6 ไฟล์; เพิ่มรูปแบบข้อมูล duty ที่ใช้กับ Serial Plotter และอธิบาย timeout ของ Serial
- [x] 2.R3 ปรับวงจร LED พื้นขาวและเพิ่มแผนภาพ delay/millis พร้อมแหล่งที่มา
- [x] 2.R4 ตรวจ compile 6 sketches, log ระดับ Info, README/code parity, quiz, template และลิงก์ แล้ว commit/push

ผลตรวจ Lab 02 รอบปรับปรุง: compile ผ่านทั้ง 6 sketches บน core 3.3.13 ด้วย `--warnings all` และ build `lab02_log_levels` เพิ่มด้วย `DebugLevel=info`; โค้ดเต็ม README ตรง `.ino`, validators และลิงก์ผ่าน เป็นการตรวจ build/เอกสาร ไม่ใช่ผลทดสอบ LED กับ hardware จริง

## Phase 3 — Lab 03
- [x] 3.1 sketches: `lab03_variables`, `lab03_button_read`, `lab03_debounce_toggle`,
      `lab03_press_counter`, `lab03_led_patterns_switch`, `lab03_state_machine`
- [x] 3.2 แผนภาพ state machine (Mermaid stateDiagram)
- [x] 3.3 คำถาม/แบบทดสอบ/เฉลย

### ปรับปรุง Lab 03 — จาก input ดิบสู่ state machine
- [x] 3.R1 ขยายพื้นฐานชนิดข้อมูล/scope/เงื่อนไข/ลูป/ฟังก์ชัน พร้อมการทดลองเป็นขั้น จุดตรวจ ตารางไล่เวลาและใบงาน
- [x] 3.R2 จัดโค้ดเต็ม 6 sketches ให้อ่านง่าย ใช้ debounce ต่อเนื่องในตัวนับ/pattern/state machine และรายงานชื่อ state
- [x] 3.R3 ปรับภาพพื้นขาววงจร/pull-up/bounce และเพิ่มกลุ่มหน้าสัมผัสปุ่ม 4 ขาพร้อมแหล่งที่มา
- [x] 3.R4 ตรวจ compile 6 sketches, README/code parity, quiz, template และลิงก์ แล้ว commit/push

ผลตรวจ Lab 03 รอบปรับปรุง: compile ผ่าน 6 sketches บน core 3.3.13 ด้วย `--warnings all`; โค้ดเต็ม README ตรง `.ino`, validators และลิงก์ผ่าน; ปรับ quiz เรื่องการกดค้างใน state machine พร้อมอัปเดตคำอธิบายเฉพาะใน `answer-keys/` ที่ยังถูก ignore ผลตรวจนี้เป็น build/เอกสาร ส่วนการกดปุ่มจริงและ contact bounce ต้องวัดกับ hardware

## Phase 4 — Lab 04
- [x] 4.1 sketches: `lab04_i2c_scanner`, `lab04_oled_test`, `lab04_button_module_test`,
      `lab04_traffic_light_test`, `lab04_button_oled`, `lab04_oled_button_traffic`
- [x] 4.2 ภาพ: pinoutสามโมดูล, วงจรแยก3ภาพ, วงจรรวม และผลลัพธ์OLEDจำลอง
- [x] 4.3 คำถาม/แบบทดสอบ/เฉลย

### ประวัติ Lab 04 แบบ LCD — ก่อนเปลี่ยนอุปกรณ์ 27 พฤศจิกายน 2026
- [x] 4.R1 เพิ่มขั้นสำรวจโมดูล/ต่อ level shifter/หา address/ติดตั้ง library แล้วสอนจอทีละคำสั่ง พร้อมจุดตรวจและใบงาน
- [x] 4.R2 ขยายโค้ดเต็ม 5 sketches ให้ตรง `.ino`: ตรวจ ACK, custom glyph/scroll timer, counter debounce และเมนูสองปุ่มแยก state
- [x] 4.R3 ปรับภาพพื้นขาว PCF8574/วงจร/พิกัดจอ และเพิ่ม bitmap 5x8 พร้อมแหล่งที่มา
- [x] 4.R4 ตรวจ quiz/เฉลย local, compile 5 sketches, README/code parity, template/ภาพ/ลิงก์ แล้ว commit/push

ผลตรวจ Lab 04 รอบปรับปรุง: compile ผ่าน 5 sketches บน Arduino-ESP32 3.3.13 ด้วย `--warnings all` และ LiquidCrystal I2C 1.1.2 (มี warning metadata architecture=avr ของ library); README/code parity ผ่าน, repository/quiz validators ผ่าน, SVG XML และ markdown-link-check ผ่าน เปลี่ยนลิงก์ datasheet ที่ตอบ 404 เป็นเอกสารผู้ผลิต TI ที่เปิดได้ ไม่ได้ upload/วัดแรงดันหรือทดสอบ LCD จริงในรอบนี้

### เปลี่ยน Lab04 เป็น OLED และโมดูลปุ่ม/ไฟ — 27 พฤศจิกายน 2026 (REQ-01–07, REQ-10)

- [x] 4.O1 เปลี่ยน scope/design/สารบัญ/BOM/pin map และลิงก์ Lab03/05/06 เป็น labs/lab04-lcd-i2c; GPIO21/22, OUT18 และ RD17/YL23/GY32 สำหรับ ESP32-WROOM-32
- [x] 4.O2 สร้าง6sketches self-contained: Scanner, OLED, ปุ่ม, ไฟ, ปุ่ม+OLED และรวมสามโมดูล; address/pressedlevel/activeLOWปรับได้และOLEDไม่พร้อมยังทดสอบSerial/ปุ่ม/ไฟได้
- [x] 4.O3 สร้างภาพ6คู่SVG/PNGพร้อมsourcegenerator: pinout, ต่อจอ, ต่อปุ่ม, ต่อไฟ, ต่อรวม และภาพผลลัพธ์จำลอง เพิ่มตาราง11สายและขั้นตอนทดสอบแยกก่อนรวม
- [x] 4.O4 เขียนบทเรียนหน้าเดียว โค้ดเต็มตรงfile คำถาม/แบบทดสอบใหม่และเฉลยlocalที่ignore แยกผลcompile/ภาพจำลองออกจากงานต่ออุปกรณ์จริง
- [x] 4.O5 ตรวจcompile6sketches, logicreview, README/code parity, ภาพ/ลิงก์/validators/ignoreแล้วcommit

ผู้สอนยืนยันชื่อพินไฟ VCC/RD/YL/GY และปุ่ม GND/VCC/OUT; จอยังไม่ทราบcontrollerจึงเลือกSSD1306 I2C128×64เป็นbaseline ต้องตรวจแรงดัน/Rจำกัดกระแส/ระดับกดจริงก่อนต่อhardware pin mapนี้ไม่รองรับS3โดยตรง โค้ดLCDและภาพLCDเดิมถูกแทนที่ ผลตรวจLCDในหัวข้อประวัติข้างต้นเป็นผลของรุ่นก่อนแก้ ไม่ใช่ผลรับรองอุปกรณ์ใหม่

ผลตรวจรอบOLED: compileผ่าน6/6sketchesด้วยArduino CLI1.6.0/core3.3.13/FQBN esp32:esp32:esp32 และ warnings all ไม่มีwarning; README/code parity6ไฟล์, logicreviewของACK/debounce/activeLOWinitและmappingผ่าน ภาพSVG/PNG6คู่ตรวจด้วยสายตา/XML/ขนาดผ่าน (วงจรรวม1600×1400; ภาพอื่น1600×1150), repository/quiz validatorsครบ7แล็บ/84ภาพ และ markdown-link-check READMEราก+Lab03/04/05/06รวม5ไฟล์ผ่าน ไม่มีuploadหรือผลทดสอบโมดูลจริงในรอบนี้

### คงตำแหน่ง Lab04 ตามลิงก์ผู้สอน — 27 พฤศจิกายน 2026 (REQ-01, REQ-05, REQ-07)

- [x] 4.P1 ย้ายบทเรียนOLED/ปุ่มGND-VCC-OUT/ไฟVCC-RD-YL-GYกลับ `labs/lab04-lcd-i2c/` และแก้สารบัญ ลิงก์บทข้างเคียง ทะเบียนภาพ และgeneratorให้ใช้พาธเดิม
- [x] 4.P2 คงโค้ดทดสอบ6ชุดและภาพ6คู่ของบทใหม่; ตรวจSHA256ของ.inoก่อนและหลังย้ายตรงกันทั้ง6ไฟล์ เนื้อหาLCD16×2/PCF8574และโค้ดLCDเก่าไม่อยู่ในบทเรียนปัจจุบัน
- [x] 4.P3 ตรวจtemplate/quiz/ลิงก์/README-code parity/ignore แล้วcommitและpush main พร้อมตรวจพาธเดิมบนorigin

ผลตรวจรอบคงพาธเดิม: validatorsผ่าน7แล็บ/84ภาพ, README/code parity6ชุดและSHA256โค้ดก่อน/หลังย้ายตรงกัน, markdown-link-check READMEราก+Lab03/04/05+QA_REPORTรวม5ไฟล์ผ่าน โค้ดชุดนี้เป็น6ไฟล์เดิมที่compileผ่านcore3.3.13ในรอบOLEDก่อนหน้า ไม่มีการแก้firmwareหรือทดสอบhardwareเพิ่ม

## Phase 5 — Lab 05
- [x] 5.1 sketches: `lab05_relay_basic`, `lab05_ultrasonic_read`,
      `lab05_ultrasonic_filter`, `lab05_distance_relay`
- [x] 5.2 ภาพ: ขั้วต่อ relay (COM/NO/NC), voltage divider, timing diagram
- [x] 5.3 กล่องคำเตือนความปลอดภัย (REQ-09)
- [x] 5.4 คำถาม/แบบทดสอบ/เฉลย

### ปรับปรุง Lab 05 — ทดลอง output/sensor ก่อนรวม
- [x] 5.R1 เพิ่มขั้นตรวจลอจิก Relay/วงจรโหลด DC, วัด echo, ไล่ circular buffer และ hysteresis พร้อมจุดตรวจ/ใบงาน
- [x] 5.R2 โค้ดเต็มตรง `.ino` 4 ไฟล์ และนโยบาย invalid ติดกัน 3 รอบปิด Relay ของโหลด LED สาธิต
- [x] 5.R3 ภาพพื้นขาวและวงจรโหลด DC พร้อมแหล่งที่มา
- [x] 5.R4 ตรวจ compile, parity, quiz/ลิงก์/ภาพ แล้ว commit/push

ผลตรวจรอบปรับปรุง Lab 05: compile ผ่าน 4 sketches บน core 3.3.13 ด้วย warnings all, README/code parity และ validators/ลิงก์ผ่าน; การทดสอบ Relay, ระยะจริง และ invalid policy กับ hardware เป็นงานในใบงาน ยังไม่ได้อ้างว่าทดสอบจริง

## Phase 6 — Lab 06
- [x] 6.1 `secrets.example.h` (WIFI_SSID, WIFI_PASS, STUDENT_ID)
- [x] 6.2 sketches: `lab06_wifi_connect`, `lab06_mqtt_basic`, `lab06_mqtt_led_control`,
      `lab06_mqtt_telemetry_json`, `lab06_mqtt_chat`
- [x] 6.3 คู่มือ MQTTX Web ทีละขั้น (Connect → Subscribe → Publish) พร้อมภาพ
- [x] 6.4 แผนภาพ sequence (Mermaid) ของ publish/subscribe
- [x] 6.5 ขั้นตอนทดสอบ end-to-end ตาม REQ-08 พร้อม expected result/checklist
- [x] 6.6 คำถาม/แบบทดสอบ/เฉลย

### ปรับปรุง Lab 06 — จาก IP สู่การส่งข้อมูลสองทิศทาง
- [x] 6.R1 สอนการเตรียม secrets/library, Wi-Fi/IP, MQTTX, heartbeat, callback/state, JSON, Chat และ LWT เป็นขั้น พร้อมจุดตรวจ/ใบงาน
- [x] 6.R2 โค้ดเต็มตรง `.ino` 5 ไฟล์, MAC 48 บิต, Wi-Fi retry, ขอบเขต payload/Chat และ log ผล publish
- [x] 6.R3 ภาพพื้นขาวและแผนภาพตรวจการเชื่อมต่อทีละชั้น พร้อมแหล่งที่มา
- [x] 6.R4 ตรวจ compile, parity, quiz/ลิงก์/ภาพ แล้ว commit/push

ผลตรวจ Lab 06 รอบปรับปรุง: compile ผ่านทั้ง 5 sketches บน core 3.3.13 ด้วย warnings all และ PubSubClient 2.8.0/ArduinoJson 7.4.3; README/code parity, repository/quiz validators, SVG XML และลิงก์ผ่าน แยกอธิบาย scheduled retry ออกจาก synchronous connect และเพิ่มใบงาน REQ-08 สำหรับทดสอบสองทิศทางบน hardware ซึ่งยังไม่ได้ทดสอบจริงในรอบนี้

## Phase 7 — Lab 07
- [x] 7.1 sketches: `lab07_oled_test`, `lab07_dht11_test`, `lab07_smart_home`
- [x] 7.2 `lab07_smart_home` แยกไฟล์: `config.h`, `secrets.example.h`, `.ino` หลัก
- [x] 7.3 ชุดสเปก React Native Android + SQLite สำหรับ OpenCode พร้อมภาพคาดการณ์ 4 หน้าและขั้นตอนสร้าง/ทดสอบภายหลัง
- [x] 7.4 วงจรรวม + ตาราง BOM
- [x] 7.5 Rubric ประเมิน + แบบฟอร์มส่งงาน
- [x] 7.6 คำถาม/แบบทดสอบ/เฉลย

### ปรับปรุง Lab 07 — ตรวจส่วนย่อยก่อนรวม Smart Home
- [x] 7.R1 สอน OLED buffer/พิกัด/ACK และ DHT/timer/NaN ก่อนรวม config/secrets/state พร้อมใบงานตรวจรับ fault
- [x] 7.R2 โค้ดเต็มตรง `.ino` และ headers, valid/stale/age, JSON null, MAC 48 บิต, Wi-Fi retry และรายงานผล publish/state เป็นระยะ
- [x] 7.R3 คู่มือแอปเปลี่ยนเป็น React Native/OpenCode ตาม REQ-11; แยกคำสั่ง/reported state/JSON quality และภาพพื้นขาว
- [x] 7.R4 ตรวจ compile, parity, quiz/ลิงก์/ภาพ แล้ว commit/push

ผลตรวจ Lab 07 รอบปรับปรุง: compile ผ่าน 3 sketches บน core 3.3.13 ด้วย warnings all; README/code/header parity, repository/quiz validators, SVG XML และลิงก์ผ่าน ตรวจภาพ flow/วงจรโดย render เป็น PNG แล้ว; ยังไม่ได้ทดสอบ OLED/DHT/Relay หรือ Dashboard กับ hardware จริง มีตารางให้ผู้เรียนบันทึกผลแต่ละกรณี รวม offline/invalid/Reset และแยก reported GPIO state ออกจากผลวัดหน้าสัมผัสจริง

### ปรับโจทย์ Lab07 — React Native / OpenCode / ข้อมูลในโทรศัพท์ (REQ-11)

- [x] 7.N1 แก้ spec/หน้า landing/README ให้ผลงานสุดท้ายเป็นแอป React Native Android และ SQLite ในโทรศัพท์
- [x] 7.N2 สร้าง app-blueprint 9 Markdown files: requirements/design/implementation/AGENTS/contract/data/test/prompts/README สำหรับคัดลอกไปโปรเจกต์ใหม่
- [x] 7.N3 Generate ภาพ 4 หน้า Dashboard/Add/Edit/Settings พื้นขาวตามสเปก พร้อม prompt/เครดิตและคำบรรยายว่าเป็นภาพคาดการณ์
- [x] 7.N4 ตรวจเอกสารทุกไฟล์/ลิงก์/quiz/contract/ภาพ และ compile firmware Lab07 ก่อน commit/push

Checklist ใน app-blueprint/implementation.md เป็นงานที่ผู้เรียนจะสร้างแอปภายหลัง จึงคง [ ] ทั้งหมด ไม่ใช่งานค้างของชุดเอกสาร และไม่มีการอ้างว่าได้ build APK/ทดสอบ Android แล้ว

### สร้างแอปอ้างอิงตามคำสั่งผู้สอน (REQ-11)

- [x] 7.A1 ตรวจ environment จากโปรเจกต์ตัวอย่าง สร้าง Expo58/React Native0.88.1/TypeScript ใน app-blueprint/mobile
- [x] 7.A2 ทำ UI4หน้า, SQLite/migration/CRUD/history/retention และ MQTT WSS/reported/pending/timeout/no-replay
- [x] 7.A3 Typecheck/lint และ automated tests19ข้อผ่าน พร้อม Android Hermes export และ Debug/Release APK
- [x] 7.A4 ทดสอบ standalone APK บน Android emulator กับ brokerจริง/ตัวจำลอง ผ่าน7กรณี จับภาพ4หน้าและตรวจ Dashboard360dp/font1.3x
- [x] 7.A5 บันทึกผลจริง วิธีรัน/build ข้อจำกัดhardware และเตรียม APK Preview/commit/push บน GitHub

แผนของผู้เรียนใน app-blueprint/implementation.md ยังเป็นแม่แบบสำหรับโปรเจกต์ใหม่ ส่วนผลของแอปอ้างอิงอยู่ที่ app-blueprint/mobile/IMPLEMENTATION.md และ TEST_RESULTS.md ไม่อ้างว่ารัน OpenCode หรือทดสอบ Relay จริงในรอบนี้

### ตรวจอุปกรณ์จริงและปรับ UI ตามผู้สอน — Lab07 (REQ-05, REQ-08, REQ-11)

- [x] 7.H1 ตรวจ ESP32-S3 Flash16MB/PSRAM8MB, สำรองFlashเฉพาะเครื่อง และเพิ่ม pin mapping S3 โดยคง ESP32 รุ่นเดิม; compile Lab07ทั้ง3sketchesบน2targets ผ่าน6builds
- [x] 7.H2 ติดตั้งและ upgrade APK1.0.1บน Samsung S25 Ultra Android17 คงข้อมูลเดิม; ปรับเส้นขอบการ์ด/ปุ่มตาม FloatingNavDemoV.2/MinimalTodoNative พร้อมแก้ spec/prompts
- [x] 7.H3 Upload/verifyจริง4sketches และตรวจโทรศัพท์↔broker↔ESP32-S3: reported ON/OFFสองช่อง/AllOff, payloadผิด, sensorinvalid, persistence/offline, timeout, LWT และ no-replay
- [x] 7.H4 จับภาพโทรศัพท์จริง4หน้า/ก่อนปรับ/ออฟไลน์ บันทึกผลดิบและข้อจำกัดตามอุปกรณ์ที่มี พร้อม APKPreview/commit/push สำหรับตรวจบน GitHub

ผู้สอนยืนยันว่าไม่มีโมดูลต่อบอร์ด ผลรอบนี้จึงไม่ครอบคลุม DHT11/OLED/Relay/โหลดจริง อ่าน app-blueprint/mobile/PHYSICAL_TEST_RESULTS.md แยกจากผล emulator และ QAเอกสารเดิม

### ตั้ง MQTTX Desktop เป็นมอนิเตอร์ — Lab07 (REQ-08, REQ-11)

- [x] 7.M1 นำเข้าgroup/connectionMQTT3.1.1 และ6 exact subscriptionsตรงnamespaceของบอร์ด/แอปในMQTTX1.13.3
- [x] 7.M2 ตั้งaliasesภาษาไทย/หมายเลขและ4หมวดสี: statusเขียว,sensorน้ำเงิน,commandส้ม,reported stateม่วง พร้อมหน้ารวมPlaintext
- [x] 7.M3 ตรวจbroker/สถานะonline/sensorheartbeat/stateจริง โดยไม่เปลี่ยนfirmwareหรือปลอมข้อความจากโทรศัพท์; ระบุว่าการกดOFFรอบนี้รอปลดล็อกโทรศัพท์
- [x] 7.M4 เก็บJSONpreset/ภาพหน้าจอจริง/คู่มืออ่านค่าและวิธีนำเข้าไว้ในmqttx/ แทรกเนื้อหาหลักในREADMEแล็บ ตรวจignore/ลิงก์/quizแล้วcommit/push

### ปรับบทเรียนให้สร้างตามระบบล่าสุด — Lab07 (REQ-05, REQ-07, REQ-08, REQ-11)

- [x] 7.D1 ปรับ README แล็บให้เรียนจบหน้าเดียว: เลือก ID/package/board/scope, baseline1.0.1, OpenCode ทีละ Phase, firmware, MQTTX, standalone APK และตรวจรับ
- [x] 7.D2 ปรับ Markdown เริ่มต้นทั้ง9ไฟล์ให้ตรง source/UI/SQLite/MQTT จริง พร้อม FW-01–10/MX-01–06, ภาพจริง4หน้าและ template MQTTX6topics/4สีที่คัดลอกแยกได้
- [x] 7.D3 แยกผลอ้างอิงของ Samsung/ESP32-S3 เปล่าจากวงจรครบและงานผู้เรียน อธิบาย QoS0/timeout/presence/retention/background/signing และสิ่งที่ยังไม่ทดสอบ
- [x] 7.D4 ตรวจชุดคัดลอก/ลิงก์/ภาพ/JSON, README/code parity, compile6builds, repository/quiz validators และ ignore rules แล้ว commit/push

ผลตรวจรอบปรับเอกสาร: คัดลอก9MD+assetsไปโฟลเดอร์ใหม่แล้วลิงก์ local28รายการ/ภาพ5สำเนา/JSON6topicsผ่าน, โค้ดเต็มตรงไฟล์ firmware5ไฟล์, compile3sketchesบนESP32และS3รวม6buildsผ่านด้วยcore3.3.13, validatorsครบ7แล็บ/55ภาพ และลิงก์เอกสาร16ไฟล์ผ่าน ชุดของผู้เรียนยังคงช่อง [ ] และต้องทดสอบโปรเจกต์ใหม่เอง ผล Android/hardware ที่ใช้อ้างอิงอยู่ในรายงาน1.0.1เดิม

### เก็บภาพผลลัพธ์สุดท้ายจากเครื่องจริง — 27 พฤศจิกายน 2026 (REQ-07, REQ-11)

- [x] 7.F1 เปิด APK1.0.1 บน Samsung S25 Ultra และจับ Dashboard ที่ออนไลน์, Add, Edit, Edit ส่วนท้าย และ Settings โดยไม่เปลี่ยน profile/Settings
- [x] 7.F2 จับหน้าต่าง MQTTX จริงที่ตั้ง6หัวข้อ/4สี หลังเชื่อมมอนิเตอร์และ Reset บอร์ดหนึ่งครั้ง รับ online/sensor/state สด; ระบุขอบเขตบอร์ดเปล่าและไม่มีคำสั่งโทรศัพท์เพิ่มรอบนี้
- [x] 7.F3 เพิ่มแกลเลอรีใน README แล็บ ลิงก์จากคู่มือแอป/MQTTX และทะเบียนภาพพร้อมขนาด/SHA256/วิธีจับภาพ
- [x] 7.F4 ตรวจภาพจริง ลิงก์/validators/ไฟล์ลับ แล้ว commit/push ภาพและเอกสารขึ้น GitHub

ผลตรวจรอบเก็บภาพ: PNGมือถือ5ไฟล์ขนาด1440×3120 และ JPEGเฉพาะหน้าต่างMQTTX1ไฟล์ขนาด2560×1552 พร้อมmanifest/SHA256 ไม่แต่งภาพ; validatorsผ่าน7แล็บ/61ภาพ, ลิงก์READMEที่เพิ่ม/แก้5ไฟล์ผ่าน ไม่มีโค้ดแอป/firmwareเปลี่ยนในรอบนี้

### ภาพต่อสาย DHT11/OLED แยก ESP32 และ S3 — Lab07 (REQ-05, REQ-07)

- [x] 7.I1 ตรวจ GPIO จาก config.h และ sketches: DHT4 ทั้งสองบอร์ด, OLED21/22 บน ESP32 และ8/9 บน S3 พร้อมตรวจเอกสาร sensor/pull-up
- [x] 7.I2 Generate ภาพพื้นขาว3ภาพ: ESP32 wiring, S3 wiring และ DHT11/OLED pin guide ตรวจปลายสาย สี ขา NC และ R10kΩ ก่อนเผยแพร่
- [x] 7.I3 เพิ่มภาพ/ตารางตรวจสาย/ขั้นตอนต่อใน README พร้อม prompt ที่บันทึกไว้ แยก functional pin map จากตำแหน่ง header จริง
- [x] 7.I4 ตรวจ compile/README-code parity/ภาพ/ลิงก์/validators/ignore แล้ว commit/push

ผลตรวจรอบภาพต่อสาย: เลือกPNG1536×1024จำนวน3ภาพ สายDHT11โมดูล3ขาไป3V3/GPIO4/GNDและสายOLED4เส้นแยกกันชัด; pin guideอธิบายsensorเปล่า4ขา/NC/R10kΩเพิ่มเติม ภาพESP32/S3เป็นfunctional GPIO mapและวาดบอร์ดซ้ำเพื่อแยกชุดสาย ใช้บอร์ดจริงเพียงตัวเดียว ตรวจcompile3sketches×2targetsผ่าน6builds, README/code parity5ไฟล์, validators7แล็บ/64ภาพ และลิงก์README/promptผ่าน ไม่อ้างว่าทดสอบต่อโมดูลจริงจากภาพAI

### ภาพต่อสายและตารางพิน Lab02–06 — 27 พฤศจิกายน 2026 (REQ-05, REQ-07, REQ-09)

- [x] 2-6.W1 ตรวจ GPIO จาก sketches ทุกชุดและแยกวงจรตามการทดลอง: LED2/16, ปุ่ม18/19, LCD21/22 ผ่าน level shifter, Relay26/27 และ HC-SR04 TRIG25/ECHO33 ผ่าน R1/R2
- [x] 2-6.W2 สร้างภาพต่อสาย 7 ภาพ PNG1600×1150 พร้อม SVG ต้นฉบับและ script สร้างซ้ำ ระบุพินปลายสาย แรงดัน กราวด์ร่วม จุด J ของ ECHO และโหลด DC ผ่าน COM/NO
- [x] 2-6.W3 เพิ่มภาพ ตารางหนึ่งสายต่อหนึ่งแถว ขั้นตอนตรวจสาย และบรรทัดวงจรที่ใช้ใต้ทุกหัวข้อทดลองใน README Lab02–06; เพิ่ม DHT11/OLED เป็นส่วนเตรียม Lab07 ท้ายหัวข้อวงจร Lab06 พร้อมลิงก์โค้ดทดสอบจริง
- [x] 2-6.W4 ตรวจภาพทุกไฟล์, README/code parity26sketches, compile core3.x, template/quiz/localและexternal links, ทะเบียนภาพและignore แล้ว commit

ภาพชุดนี้เป็นผังการเชื่อมต่อจากชื่อพิน ไม่ใช่ผังตำแหน่ง header ที่ใช้ได้กับทุกบอร์ด สร้างเองด้วย SVG และ render เป็น PNG ไม่ใช้ภาพ AI หรือภาพถ่ายเป็นหลักฐานการทำงานของอุปกรณ์ การอ่าน DHT11/OLED ยังเป็นการทดลองใน Lab07 ตามโครงสร้างคอร์ส

ผลตรวจ: compile ผ่าน26/26sketchesด้วยArduino CLI1.6.0/core3.3.13/FQBN esp32:esp32:esp32 และ warnings all จากชุด environment เดิม (default core3.3.11 มี toolchain error จึงไม่ได้ใช้เป็นผลรับรอง); มีเพียง metadata warning architecture=avr ของ LiquidCrystal I2C ใน4sketches จอ โค้ดเต็มREADMEตรงไฟล์ทั้ง26ชุด มีคำอธิบายวงจรใต้ทุกหัวข้อทดลอง ภาพ7คู่SVG/PNGผ่านXML/ขนาด1600×1150และตรวจด้วยสายตา validatorsผ่าน7แล็บ/78ภาพและลิงก์READMEทั้ง5บทผ่าน ไม่ได้uploadหรือทดสอบวงจรกับอุปกรณ์จริงในรอบนี้

### พินคงที่ ESP32-S3 ตลอด Lab02–07 — 27 พฤศจิกายน 2026 (REQ-05, REQ-06, REQ-07)

- [x] 2-7.S1 ยืนยันผู้สอนใช้ESP32-S3และยึดLab07เดิม: OLED8/9, Relay16/17, DHT4; ไม่แก้ไฟล์ใดในLab07
- [x] 2-7.S2 แก้Lab02–06ให้เลือกS3/WROOMตามtarget: LEDหลัก5/เสริม2, ปุ่มโมดูลOUT18, ไฟRD10/YL11/GY12, HC-SR04TRIG6/ECHO7ผ่านdivider; ใช้พินเดิมทุกsketchและโมดูลปุ่มตัวเดิมตั้งแต่Lab03ถึง04
- [x] 2-7.S3 ปรับREADME/โค้ดเต็ม/ภาพหลักS3/สารบัญ/สเปก พร้อมตารางVCC-GNDและทุกสายที่assets/PINOUT.md แยกWROOMlegacyแบบพับเก็บ ไม่ใช้รูปWROOMเป็นภาพวงจรหลักS3
- [x] 2-7.S4 เพิ่มscripts/validate-pinout.pyตรวจS3/WROOMconstants/LEDarray/พินไม่ชน/README-code parity30sketches และตรวจLab07ไม่มีdiff
- [x] 2-7.S5 Compile27sketchesLab02–06บนS3และตัวแทนWROOM6ชุด ตรวจภาพ/ลิงก์/validators/ignoreแล้วcommit/pushพร้อมรายงานขอบเขตhardware

ตารางพินคงที่ช่วยคงสายสัญญาณเดิมเมื่อเปลี่ยนบท ไม่ได้ยืนยันว่าอุปกรณ์ที่sketchไม่ใช้จะOFFช่วงResetหรือก่อนsetup ผลhardwarenั้นยังอยู่ในใบงาน ไม่ปรับLab07เพื่อเพิ่มการควบคุมโมดูลจากบทก่อนหน้า

ผลตรวจรอบพินS3: CompileLab02–06ครบ27/27sketchesบนArduino-ESP32core3.3.13/CLI1.6.0 ด้วยFQBN esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi,CDCOnBoot=default,UploadSpeed=115200 และ warnings all ผ่านไม่มีwarning; ตัวแทนWROOM6/6ผ่านไม่มีwarning SHA256ไฟล์ที่compileตรงsourceปัจจุบันทุกชุด ตัวตรวจพิน/README-code parity30sketchesและvalidators7แล็บ/84ภาพผ่าน ลิงก์READMEราก+Lab02–06+PINOUTรวม7ไฟล์และQA_REPORTผ่าน ตรวจLab07ทั้งโฟลเดอร์ไม่มีdiffจากe23e16f (tree abccc18214fd9c8c914863f9e1716f29b89745a6) ไม่มีuploadหรือการทดสอบอุปกรณ์ต่อค้างจริงในรอบนี้

### ชุดโค้ดทดสอบโมดูลแยก — 27 พฤศจิกายน 2026 (REQ-05, REQ-06)

- [x] M.T1 สร้าง5sketchesESP32-S3ในmodule-tests/: DHT11/Ultrasonic/ไฟสามสี/Relay2CH/ปุ่มโมดูล พินคงที่ตรงassets/PINOUT.mdและไม่แก้Lab07
- [x] M.T2 เพิ่มคู่มือต่อสาย VCC/GND/พิน/ตัวเลือกactiveLOWและpressedlevel ตัวอย่างSerial และลิงก์จากREADMEราก
- [x] M.T3 ตรวจcompile5ชุด, พิน/ลิงก์/ZIPตรงsourceและignore แล้วcommit/pushพร้อมไฟล์ดาวน์โหลดรวม

ผลตรวจชุดแยก: compile5/5ผ่านด้วยCLI1.6.0/core3.3.13/FQBN S3ของผู้สอนและ warnings all ไม่มีwarning SHA256ตรงsourceปัจจุบันทั้งหมด ตัวตรวจพิน5โมดูลผ่าน คู่มือลิงก์ผ่าน ZIPมี5.ino+READMEรวม6รายการและbytesตรงต้นฉบับ ไม่มีsecrets/answer-keys/Lab07เปลี่ยน ไม่ได้uploadหรือทดสอบโมดูลจริง

## Phase 8 — Lab08 ตรวจเช็กอุปกรณ์ก่อนใช้งาน (เพิ่มตามคำสั่งผู้สอน)

- [x] 8.H1 เพิ่มบทพิเศษที่ใช้ก่อนบทอื่นได้ รักษาพินS3เดิมและไม่แก้Lab07
- [x] 8.H2 สร้าง6sketchesแยก DHT/HC-SR04/ไฟสามสี/Relay/ปุ่ม/OLEDข้อความ+UI พร้อมปุ่มZIPรายโมดูลและรวม
- [x] 8.H3 ภาพต่อสายครบ6โมดูล/overview/UIresultและตารางสายทุกเส้น พร้อมทะเบียนภาพและsourceสร้างซ้ำ
- [x] 8.H4 READMEหน้าเดียว โค้ดเต็ม/จุดตรวจ/โจทย์ต่อยอดพร้อมแนวทางไม่มีเฉลยของโจทย์ ส่วนRelayทดสอบเปิด–ปิดเท่านั้น; Prelab/Research/Quizและเฉลยlocal
- [x] 8.H5 Compile6ชุด ตรวจภาพ/พิน/โค้ดREADME/ZIP/ลิงก์/template/quiz/ignore แล้วcommit/push

ผลตรวจLab08: compile6/6sketchesผ่านcore3.3.13/CLI1.6.0/FQBNS3ของผู้สอนและwarnings all ไม่มีwarning SHA256ตรงsource โค้ดREADMEตรง6ไฟล์/พินตรงคอร์ส/ZIP7ชุดCRCและbytesผ่าน ภาพ8คู่SVG-PNGและSVGปุ่มรวม17ไฟล์ตรวจด้วยสายตา/XML/ขนาดผ่าน repository/quiz validatorsผ่าน8แล็บ/101ภาพ ลิงก์READMEแล็บ/รากผ่าน ปุ่มZIPครบ7ปุ่มชี้ไฟล์จริง เฉลยlocalถูกignoreและไม่มีโค้ดเฉลยโจทย์ต่อยอดในREADME ไม่แก้Lab07 ไม่uploadหรือทดสอบอุปกรณ์จริง

### เพิ่ม OLED ในชุด module-tests — 27 พฤศจิกายน 2026 (REQ-05, REQ-06)

- [x] M.O1 เพิ่มesp32s3_oled_test.inoแยกในmodule-testsพร้อมคู่มือต่อ3V3/GND/SDA8/SCL9และlibrary ให้ใช้กับOLEDเพียงตัวเดียว
- [x] M.O2 อัปเดตZIPรวม/สารบัญ/สเปก/ตัวตรวจพินเป็น6โมดูล โดยคงไฟล์ทดสอบ5ชุดเดิมและLab07/08
- [x] M.O3 CompileOLEDบนS3 ตรวจZIP7รายการตรงsource/พิน/ลิงก์/ignoreแล้วcommit/push

ผลตรวจเพิ่มOLEDในmodule-tests: compileOLEDบนCLI1.6.0/core3.3.13/FQBNS3เดิมด้วยwarnings allผ่าน ไม่มีwarning; ตัวตรวจพินครบ6โมดูล ลิงก์คู่มือ/READMEรากผ่าน ZIPมี6.ino+READMEรวม7รายการตรงsource CRCผ่าน ไฟล์.ino5ชุดเดิมและLab07/08ไม่มีdiff ไม่มีuploadหรือทดสอบกับจอจริง

### เปลี่ยน driver ทดสอบ OLED เป็น SH1106 — 27 พฤศจิกายน 2026 (REQ-05, REQ-06, REQ-10)

- [x] M.S1 เปลี่ยนค่าเริ่มต้นesp32s3_oled_testเป็นSH1106/SH1106Gตามจอผู้สอน มีตัวเลือกSSD1306โดยคงSDA8/SCL9และตรวจACK0x3C/0x3D
- [x] M.S2 ใช้กรอบ/ข้อความคงที่และuptimeแทนแถบDEMO พร้อมคู่มือlibrary/เลือกdriver/แก้ปัญหา ระบุขอบเขตตัวอย่างเดิมในLab04/07/08
- [x] M.S3 Compileทั้งสองdriverบนS3 ตรวจZIPรวมตรงsource พิน/ลิงก์/ignore แล้วcommit/push

ผลตรวจเปลี่ยนdriverในmodule-tests: SH1106Gค่าเริ่มต้นและSSD1306ผ่านCLI1.6.0/core3.3.13/FQBNS3เดิมด้วยwarnings all ไม่มีwarning ใช้AdafruitSH110X2.1.15/GFX2.0.0/BusIO1.17.4/SSD13062.5.17; ตัวเลือกSSD1306ตรวจด้วยcompiler.cpp.extra_flags=-DOLED_DRIVER=2โดยคงflagsของบอร์ด ตัวตรวจพิน/โค้ดREADME/นโยบายไฟล์และลิงก์คู่มือ/รากผ่าน ZIP6.ino+READMEรวม7รายการCRC/bytesตรงsource อีก5sketchesและทุกไฟล์ในlabsไม่เปลี่ยน ไม่มีuploadหรือยืนยันภาพบนจอจริง

## Phase 9 — Lab09 S3 HomeGuard (REQ-12)

- [x] 9.1 อ่านLevel5/สำรวจLab07 กำหนดHG01–12/สองวิธีpairing/brokerเปลี่ยนได้ พร้อมDECISIONSและแยกงานsecure runtimeที่ยังต้องพัฒนา
- [x] 9.2 READMEปฏิบัติสั้น/ขั้นตอน/จุดตรวจตามผู้สอน มีPrelab/Research/Quizแบบพับและเฉลยlocal
- [x] 9.3 ArduinoS3preflight/crypto/TLSprobe compileผ่าน พร้อมfullcode/downloadZIPไม่มีsecret
- [x] 9.4 Appstarter/migrationhelperชื่อpackageiconใหม่/Relayล็อก/publicconfigexport, typecheckและAndroidbundleexport
- [x] 9.5 ภาพ/manifest/พิน/ลิงก์/validators/protocolnegative tests/รายงานขอบเขตจริงแล้วcommit

ผลตรวจLab09: ArduinoS3ทั้ง3sketchescompileผ่านcore3.3.13/warnings all ไม่มีwarning; Pythonreference11testsผ่าน; migrationhelper/typecheck/Androidbundleผ่าน; PINOUT/README-code/ZIP5รายการ/ภาพ8ไฟล์/validators9บท109ภาพ/ลิงก์ผ่าน รายละเอียดในlabs/lab09-secure-homeguard/TEST_REPORT.md ไม่แก้Lab07 ไม่มีhardwareupload/nativepairing/secureMQTTend-to-end/productionhardeningในผลรอบนี้

### แอปและ development runtime เพิ่มตามผู้สอน (REQ-12)

- [x] 9.A1 สร้าง native Kotlin/Keystore/AEAD/session/MQTT/Security2 bridge, React Native4หน้า, broker profiles, public export และ tests
- [x] 9.A2 Build standalone APK และติดตั้ง Samsung S25 Ultra; ทดสอบ crypto/TLS บนโทรศัพท์จริงและ emulator แยก พร้อม source/lockfile/build script
- [x] 9.A3 Build/upload S3 development runtime พินเดิม สำรองFlashก่อนเขียน ตรวจ Wi-Fi/GPIO OFF/BLE discovery บนบอร์ดเปล่า พร้อมระบุไม่มีโมดูลจริง
- [ ] 9.A4 ตรวจจับคู่ BLE/SoftAP ครบ, AEAD command/ACK, replay/lease/reboot และ broker migration จริง แล้วอัปเดตผลตามหลักฐาน

- [x] 9.A5 ปรับหน้าเพิ่มอุปกรณ์ให้ปุ่มจับคู่ค้างเหนือเมนูล่าง พร้อมสถานะและคำแนะนำ ติดตั้งAPK2.0.1แล้ว ตรวจตำแหน่งปุ่มขณะเลื่อนฟอร์ม/typecheck/lint/buildผ่าน
- [x] 9.A6 ค้นเอกสารทางการ/เทียบSDKกับGATT log แก้descriptorที่หายและlifecycle เพิ่มregressionที่จับบั๊กเดิมได้ ตรวจnative9/9/build/install2.0.3/compile-flashfirmwareผ่าน พร้อมรายงานแยกผลจริงกับงานpairingที่ยังไม่ครบ
- [x] 9.A7 แยกทดสอบแพ็กเกต SRP ใน release APK บน Samsung และ emulator พบข้อผิดพลาดใน 2.0.4 และทดสอบผ่านบน 2.0.5 หลังเพิ่มกฎ R8 ตรวจและส่งต่อข้อผิดพลาด handshake และลดการใช้ stack ที่ endpoint ของบอร์ด
- [x] 9.A8 ยืนยันการจับคู่ BLE Security2 จาก Samsung กับ ESP32-S3 บอร์ดเปล่า บันทึกเจ้าของ เปิดเซสชันผ่าน EMQX TLS และสั่งเปิด–ปิด GPIO16/17 พร้อมตรวจค่าที่พินและสถานะบนหน้าจอ
- [x] 9.A9 ปรับหน้าควบคุมเป็นการ์ดคู่โทนขาว–น้ำเงิน ใช้ภาพประกอบ IoT เมนูล่างลอย 5 ปุ่มและรวมการตั้งค่า เปิดการยืนยันจากการ์ดรีเลย์ สร้าง APK 2.1.1 และตรวจเมนู ฟอร์มจับคู่ และ crypto self-test บน Android Emulator
- [x] 9.A10 เปิด Android แบบเต็มจอ ยึดเมนู 5 ปุ่มระดับเดียวกันที่ขอบล่าง ไฮไลต์วงกลมตามแท็บ และแก้ภาพบ้านให้พอดีกรอบ ติดตั้ง 2.1.2 บน Samsung และตรวจหน้าหลัก การสลับแท็บ ฟอร์มจับคู่ และคีย์บอร์ด
- [x] 9.A11 เปลี่ยนเมนูล่างเป็นตัวอย่างที่ 4 Compact Center จาก FloatingNavDemoV.2: ปุ่มกลาง 50dp แท็บด้านข้างเปลี่ยนสีไอคอน/ข้อความ ไม่มีเงาหรือวงกลมไฮไลต์ ติดตั้ง APK 2.1.3 บน Samsung ตรวจครบห้าแท็บและตำแหน่งคงที่; typecheck/lint/TypeScript3/JVM11/build/signature/ตัวตรวจพินผ่าน
- [x] 9.A12 เขียน SYSTEM_WORKFLOW.md อธิบาย boot/pairing/Session/คำสั่ง/ACK/State/Lease ตาม source พร้อม Mermaid ตัวอย่าง JSON/envelope ที่คำนวณจริง คำสั่งทดลอง เปรียบเทียบ Lab07 ข้อจำกัดและอ้างอิงทางการ; ตรวจ JSON17กรอบ/envelope3ชุด/Pythonตัวอย่าง/reference11tests/พิน/ลิงก์ภายในผ่าน ไม่มีการทดสอบ hardware เพิ่ม
- [x] 9.A13 ปรับMarkdown27ไฟล์ของLab09/สเปกราก แยกreference/starter เพิ่มBUILD_GUIDE/CONFIGURATION/USER_GUIDE/KNOWN_ISSUES นำGATT/R8/lifecycle/stack/UIมาเป็นgate ปรับschema/contract/status/prompts ตรวจ216ลิงก์ภายใน/JSON20กรอบ/SQL/พิน/typecheck/lint/JS3/Python11/JVM11ผ่าน ไม่มีhardwareผลใหม่ (REQ-12)
- [x] 9.A14 จับภาพจริงแอป2.1.3บนSamsung6หน้า เพิ่มแกลเลอรีท้ายREADMEแล็บ/ชุดพัฒนา พร้อมคำอธิบาย/ลิงก์ภาพเต็ม/ทะเบียนภาพ ตรวจภาพไม่มีsecretและคงFLAG_SECURE ตรวจPNG/ลิงก์/validatorsแล้วcommit (REQ-07, REQ-12)

## Phase 10 — QA (เดิม Phase8 ก่อนเพิ่มLab08/09)
- [x] 8.1 ตรวจ template ครบทุกแล็บ (REQ-01)
- [x] 8.2 ตรวจว่า Pre-lab ไม่มีคำตอบ (REQ-02)
- [x] 8.3 ตรวจการเรียงความยากและการกระจายตำแหน่งคำตอบ (REQ-04)
- [x] 8.4 compile ทุก sketch: `arduino-cli compile --fqbn esp32:esp32:esp32 <path>`
- [x] 8.5 ตรวจ IMAGE_SOURCES.md ครบทุกภาพ และไม่มีไฟล์จาก `_reference/` หลุดเข้า git
- [x] 8.6 ตรวจลิงก์ README สำหรับผู้เรียนทั้งหมดด้วย `markdown-link-check`

### QA หลังปรับบทเรียนครบทั้งคอร์ส — 26 พฤศจิกายน 2026

- [x] 8.R1 compile ซ้ำครบ 30 sketches บน core 3.3.13 ด้วย warnings all
- [x] 8.R2 ตรวจ README/code parity ครบ 30 ไฟล์, template/quiz/research keywords ครบ 7 บท และภาพ 37 ไฟล์
- [x] 8.R3 ตรวจลิงก์ README รากและทุกบท พร้อมตรวจ ignore rules/tracked files/history ของโฟลเดอร์เฉลยและข้อมูลลับ
- [x] 8.R4 บันทึกสภาพแวดล้อม ผลตรวจ warning และขอบเขต hardware ที่ยังไม่ได้ทดสอบใน assets/QA_REPORT.md พร้อมลิงก์จากหน้า landing แล้ว commit/push

## Definition of Done (ต่อแล็บ)
- README ครบตาม template, โค้ด compile ผ่าน, มีภาพ ≥3 ภาพ พร้อมแหล่งที่มา,
  แบบทดสอบ 10 ข้อ + เฉลย, Troubleshooting ≥5 รายการ

## นโยบายเผยแพร่ตามคำสั่งผู้สอน

- [x] PUB-01 แยก source/ประวัติผู้สอนเป็น Private; ชุดสาธารณะใช้บทเรียนและภาพเดิม ไม่มี implementation แอปสำเร็จ/แท็กเก่า/GitHub Releases แจกเฉพาะ APKพื้นฐาน1.0.1ผ่านdownloads และตรวจpolicy/pinout/links/ภาพก่อนเผยแพร่ (REQ-11, REQ-12)

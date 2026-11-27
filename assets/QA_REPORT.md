# ผลตรวจคอร์ส ESP32 IoT Lab Series

## เพิ่ม OLED ในชุด module-tests — 27 พฤศจิกายน 2026

เพิ่ม[esp32s3_oled_test.ino](../module-tests/esp32s3_oled_test/esp32s3_oled_test.ino) ใช้จอSSD1306 I2C128×64เพียงตัวเดียว VCC3V3/GND/SDA8/SCL9 แสดงOLED READYและUIชื่อ/uptime/DEMO [ZIPชุดเดิม](../module-tests/esp32s3-module-tests.zip) อัปเดตเป็น6.ino+READMEรวม7รายการ ตรวจCRCและbytesตรงsource; compileไฟล์ใหม่ผ่านCLI1.6.0/core3.3.13/FQBNS3เดิม/warnings allโดยไม่มีwarning ตัวตรวจพิน6โมดูลและลิงก์คู่มือ/READMEรากผ่าน

ไฟล์ทดสอบ5ชุดเดิมและLab07/08ไม่เปลี่ยน ไม่มีUpload/ทดสอบจอจริง Logs/SHA256อยู่build/module-tests-qa/oled-summary.jsonและesp32s3_oled_test.logที่ignore ผลชุด5โมดูลในหัวข้อประวัติด้านล่างเป็นผลก่อนเพิ่มOLED

## Lab08 ตรวจเช็กอุปกรณ์ก่อนใช้งาน — 27 พฤศจิกายน 2026

[บทพิเศษLab08](../labs/lab08-hardware-check/README.md) รวมDHT11/HC-SR04/ไฟสามสี/Relay2CH/ปุ่ม/OLEDข้อความและUI ใช้หลังเตรียมIDEตามLab01ได้ก่อนบทอื่น มีภาพและตารางพินครบ6โมดูล ปุ่มZIPรายโมดูล6และZIPรวม1 โจทย์ต่อยอดมีแนวทางโดยไม่แจกโค้ดคำตอบ ส่วนRelayตรวจเปิด–ปิดอย่างเดียว

- Compile6/6ชุดผ่านCLI1.6.0/core3.3.13/targetS3 Flash16M,OPI,CDCDisabled,warnings all ไม่มีwarning; SHA256sourceตรงกับที่buildทุกไฟล์ Logsอยู่build/lab08-qa/ซึ่งignore
- ตัวตรวจพินและREADME/code parity6ชุดใหม่ผ่าน เพิ่มจาก30ชุดLab02–07และ5ชุดstandaloneเดิม; repository/quiz validatorsผ่าน8แล็บ/101ภาพ
- ภาพ8คู่SVG-PNGและSVGปุ่มรวม17ไฟล์มีทะเบียน/แหล่งsource [generator](../scripts/generate-lab08-images.py) ตรวจภาพ/XML/ขนาดแล้ว; OLEDUIเป็นภาพจำลองตามcode ไม่ใช่ภาพฮาร์ดแวร์
- ตรวจZIP7ไฟล์CRC/เนื้อหาตรง.ino, downloadlinks7ปุ่มชี้ไฟล์จริง, READMEแล็บและรากลิงก์ผ่าน; เฉลยLab08อยู่เฉพาะlocalที่ignore
- Lab07ไม่เปลี่ยน ไม่มีUploadหรือผลวัดโมดูลจริง ตัวอย่างSerial/UIและผลcompileใช้แทนการกรอกใบงานว่าฮาร์ดแวร์ผ่านไม่ได้


## ชุดทดสอบโมดูลแยก — 27 พฤศจิกายน 2026

เพิ่ม [module-tests](../module-tests/README.md) และ [ZIPรวม5สเก็ตช์](../module-tests/esp32s3-module-tests.zip) ตามคำขอผู้สอน แยกจากสเก็ตช์บทเรียนเดิม ใช้ ESP32-S3/pin mapเดิมและSerial115200 มี DHT11, HC-SR04, ไฟRD/YL/GY, Relay2CH และปุ่มGND/VCC/OUT

- Compile5/5ผ่านไม่มีwarningด้วยArduinoCLI1.6.0/core3.3.13, `--warnings all`, FQBN `esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi,CDCOnBoot=default,UploadSpeed=115200`; SHA256ของsourceก่อน/หลังbuildตรงปัจจุบันทั้ง5ไฟล์
- ZIPมีREADMEและ5sketchesในโฟลเดอร์ชื่อเดียวกับ.ino รวม6รายการ ตรวจCRC/bytesตรงsource; คู่มือครอบคลุมการเลือกBoard/library/พินไฟ/กราวด์/dividerและผลSerialคาดหวัง
- ตัวตรวจพินเพิ่มการตรวจชุดแยก5ไฟล์ พร้อมguardS3และชื่อโฟลเดอร์; repositoryvalidatorยังผ่าน7แล็บ/84ภาพ ลิงก์คู่มือผ่าน
- Logs/summaryอยู่ `build/module-tests-qa/` ที่ถูกignore ไม่มีการแก้Lab07หรือการUpload/ทดสอบอุปกรณ์จริงในรอบนี้


## พินคงที่ ESP32-S3 ตลอด Lab02–07 — 27 พฤศจิกายน 2026

ผู้สอนยืนยันใช้ESP32-S3 จึงใช้[ตารางพินคงที่ทั้งคอร์ส](PINOUT.md)และรักษาLab07เดิมทุกไฟล์: DHT4/OLED8,9/Relay16,17 ส่วนLab02–06ใช้LED5/เสริม2, โมดูลปุ่มOUT18, ไฟRD10/YL11/GY12 และHC-SR04TRIG6/ECHO7ผ่านdivider แยกWROOMlegacyตามtargetและรายละเอียดพับเก็บ

เพิ่ม[ตัวตรวจพิน](../scripts/validate-pinout.py)เพื่อเทียบconstantsทั้งสองtargetกับสัญญาพิน ตรวจLEDarray/การชนและREADME-code parity30sketches รายงานรอบLCD/WROOMและOLEDWROOMด้านล่างเป็นประวัติก่อนผู้สอนยืนยันS3 ไม่ใช้พินเหล่านั้นแทนตารางหลักปัจจุบัน

ผลตรวจรอบนี้:

- Compile S3ครบ27/27sketches: Lab02=6, Lab03=6, Lab04=6, Lab05=4, Lab06=5 ผ่านด้วยCLI1.6.0/core3.3.13 และ `--warnings all` ไม่มีwarning ใช้FQBNเดียวกับบอร์ดอ้างอิงLab07 `esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi,CDCOnBoot=default,UploadSpeed=115200`
- Compile WROOMตัวแทน6/6ผ่านไม่มีwarning: Blink, PWM, patterns, OLED+ปุ่ม+ไฟ, ระยะ+Relay และMQTT LED; ไม่อ้างว่าตรวจWROOMครบทุกsketchในรอบนี้
- เก็บSHA256ก่อน/หลังbuild และเทียบกับsourceปัจจุบันครบ33buildsตรงกัน Logsอยู่ `build/pinout-audit/s3-lab02-03-06/` และ `build/pinout-s3-lab04-05/` ซึ่งถูกignore
- `python scripts/validate-pinout.py` ผ่านconstantsทั้งสองtarget, ชุดพินS3ไม่ชน, LEDarray และREADME/code parity30sketches; repository/quiz validatorsผ่าน7แล็บ/84ภาพ; SVG XML/ขนาดPNGตรงSVGและภาพหลักS3ตรวจด้วยสายตาแล้ว
- ลิงก์READMEราก+Lab02–06+PINOUTรวม7ไฟล์ผ่านตาม `.markdown-link-check.json` และQA_REPORTผ่านอีก1ไฟล์ ใช้exceptionsเดิมของSilabs/MQTTXWeb/WCH ไม่มีเพิ่มข้อยกเว้น
- Lab07ทุกไฟล์เหมือนcommitก่อนปรับ `e23e16f` และมีtreeSHA `abccc18214fd9c8c914863f9e1716f29b89745a6`; ไม่มีupload, อ่านเซ็นเซอร์จริง, วัดแรงดัน/กระแส หรือทดสอบอุปกรณ์ต่อค้างในรอบนี้



## อัปเดต Lab04 OLED / โมดูลปุ่ม / ไฟสามสี — 27 พฤศจิกายน 2026

คงตำแหน่งบทเรียนที่ `labs/lab04-lcd-i2c/` ตามลิงก์เดิมของผู้สอน ชื่อพาธเดิมไม่เปลี่ยนอุปกรณ์ในบทซึ่งใช้ OLED/ปุ่มโมดูล/ไฟสามสี ตรวจSHA256ของโค้ด6ไฟล์ก่อนและหลังย้ายตรงกัน และตรวจลิงก์อีกครั้งแล้ว

บทเรียนปัจจุบันอยู่ที่ [Lab04 OLED และโมดูลปุ่ม/ไฟ](../labs/lab04-lcd-i2c/README.md) เปลี่ยนจาก LCD เป็น baseline SSD1306 I2C128×64 พร้อมปุ่ม GND/VCC/OUT และไฟ VCC/RD/YL/GY ตามที่ผู้สอนแจ้ง ใช้ ESP32-WROOM-32, SDA21/SCL22, OUT18, RD17/YL23/GY32 และอุปกรณ์ที่รองรับ3.3V/กระแสGPIOตามข้อกำหนดบทเรียน

- Compile6/6sketchesผ่านด้วย Arduino CLI1.6.0, core3.3.13, FQBN esp32:esp32:esp32 และ `--warnings all` ไม่มีwarning ใช้environment3.3.13เดิม ไม่ติดตั้งหรืออัปเดตเพื่อรอบนี้
- โค้ดเต็มREADMEตรงไฟล์6sketches; ตรวจACKแยกจากOLEDinit, debounce40ms, กดค้างไม่เพิ่มซ้ำ, วงจรไฟactiveLOWและการเตรียมGPIOตามAPIcore3.xโดยreview source ไม่ใช่ผลวัดฮาร์ดแวร์
- ภาพ6คู่SVG/PNGมีทะเบียนแหล่งที่มาและsourceสร้างซ้ำ: [generator Lab04](../scripts/generate-lab04-diagrams.py) เรียกผ่าน [generatorรวม](../scripts/generate-wiring-diagrams.py) ผลลัพธ์OLEDเป็นภาพจำลองตามโค้ด ไม่ใช่ภาพถ่ายจอ
- Repository/quiz validatorsผ่าน7แล็บ/84ไฟล์ภาพ; README/code parity6ชุด, SVG XML/PNGขนาดและภาพทุกไฟล์ผ่าน; ลิงก์READMEราก+Lab03/04/05/06รวม5ไฟล์ผ่าน; เฉลยยังอยู่เฉพาะlocalที่ถูกignore
- Logsที่สร้างในเครื่องอยู่ `build/lab04-oled/` ซึ่งไม่เผยแพร่: `summary.json`, logs6sketches และ `links/` ปัจจุบันทั้งคอร์สมี31sketches แต่การcompileรอบนี้ครอบคลุมเฉพาะ6ชุดที่เปลี่ยนในLab04

ยังไม่ได้ต่อ/Upload/อ่านจอหรือปุ่มหรือวัดกระแสไฟสามสีจริง ต้องตรวจcontroller/address OLED, ระดับOUTตอนกด/ปล่อย, Rจำกัดกระแสและแรงดัน/ขั้วร่วมของไฟ รวมถึงสถานะขณะResetและการต่อค้างกับแล็บอื่นตามใบงาน ข้อมูลผลLCD/จำนวนsketches/ภาพในรายงานเดิมด้านล่างเป็นประวัติก่อนเปลี่ยนอุปกรณ์ ไม่ใช่ผลรับรองLab04รุ่นใหม่


ตรวจวันที่ **26 พฤศจิกายน 2026** หลังปรับบทเรียน Lab 01–07 เป็นการเรียนแบบทีละขั้น เนื้อหาหลักอยู่ใน README ของแต่ละบท พร้อม code block เต็มและลิงก์ `.ino` สำหรับเปิดใน IDE

## รอบสร้างแอป React Native อ้างอิง

ตามคำสั่งเพิ่มเติม สร้าง source ใน [mobile/](../labs/lab07-smart-home-project/app-blueprint/mobile/README.md) และ APK standalone แล้ว: typecheck/lint, automated tests19ข้อ, Android export, Debug/Release build และ Android release smoke7กรณีผ่าน ภาพ native4หน้าเพิ่มใน evidence/ พร้อม source ของไอคอนรวม2ไฟล์ ตรวจ Dashboard360dp/font1.3xแล้ว

ผล MQTT runtime ใช้ public brokerจริงกับ **protocol simulator** ไม่ใช่ ESP32/Relay จริง รายละเอียดรุ่น คำสั่ง SHA256 และสิ่งที่ยังไม่ทดสอบอยู่ใน [ผลตรวจแอป](../labs/lab07-smart-home-project/app-blueprint/mobile/TEST_RESULTS.md) งานในรอบเตรียมสเปกด้านล่างเป็นบันทึกก่อนสร้างแอปอ้างอิง

## รอบแก้โจทย์ Lab07 — React Native/OpenCode (REQ-11)

- เตรียม `app-blueprint/` จำนวน 9 Markdown files พร้อมภาพคาดการณ์ 4 หน้าให้คัดลอกไปโปรเจกต์ใหม่
- แก้ scope/spec/README/Rubric/Quiz ให้จบที่ React Native Android + SQLite ในโทรศัพท์ ใช้ WSS กับ EMQX และ MQTTX เป็น client ตรวจข้อความ
- ตรวจลิงก์ README ราก, README Lab07 และ Markdown ในชุดสเปกทั้ง 9 ไฟล์ผ่านรวม 11 ไฟล์ ตรวจ local links ภายในชุดว่าไม่ต้องพึ่ง path ภายนอกหลังคัดลอก
- ตรวจ inventory, contract กับ firmware, checklist ของผู้เรียนยังว่าง, repository/quiz validators และ README/code/header parity ผ่าน
- Compile ซ้ำ firmware Lab07 ทั้ง 3 sketches ผ่านบน core 3.3.13 ด้วย warnings all ไม่มีการเปลี่ยน source firmware จากรอบตรวจ 30 sketches ด้านล่าง
- จำนวนภาพรวมยัง 37 ไฟล์: แทนที่ภาพ dashboard SVG เดิมด้วย PNG ที่ generate ใหม่ใน app-blueprint/assets พร้อม prompt/เครดิต ตรวจภาพด้วยสายตาและ render แผนภาพสถาปัตยกรรมที่แก้แล้ว
- **ในรอบเตรียมสเปกนั้นยังไม่มี source React Native/APK หรือผลรัน Android/MQTT ของแอป** เพราะงานในช่วงนั้นคือเตรียมชุดสเปกและภาพเป้าหมาย ผู้สอนจะพานักเรียนสร้างและทดสอบภายหลังตาม [แผนผู้เรียน](../labs/lab07-smart-home-project/app-blueprint/implementation.md) และ [test-plan](../labs/lab07-smart-home-project/app-blueprint/test-plan.md)

## สภาพแวดล้อมที่ตรวจ

- Windows, Arduino CLI **1.6.0** ที่ติดตั้งมากับ Arduino IDE
- Arduino-ESP32 **3.3.13**, FQBN **esp32:esp32:esp32**
- ทุก sketch ใช้ `--warnings all`; compile ด้วย `secrets.example.h` ไม่สร้างไฟล์รหัสผ่านจริง
- Library ที่ใช้: PubSubClient 2.8.0, ArduinoJson 7.4.3, LiquidCrystal I2C 1.1.2, Adafruit SSD1306 2.5.17, Adafruit GFX 2.0.0, Adafruit BusIO 1.17.4, DHT sensor library 1.4.7, Adafruit Unified Sensor 1.1.15

## ผลตรวจ

| รายการ | ขอบเขต | ผล |
|---|---|---|
| Compile core 3.x (REQ-05) | 30 sketches: Lab01=1, Lab02=6, Lab03=6, Lab04=5, Lab05=4, Lab06=5, Lab07=3 | ผ่าน |
| โค้ดเต็มใน README (REQ-06) | เทียบข้อความกับ `.ino` ทั้ง 30 ไฟล์ และ headers ของ Lab07 | ตรงกัน |
| Template/Troubleshooting (REQ-01/10) | หัวข้อตามลำดับและตารางแก้ปัญหาอย่างน้อย 5 แถวทั้ง 7 บท | ผ่าน |
| Pre-Lab (REQ-02) | คำถามล้วนใน details จำนวน 5–8 ข้อต่อบท | ผ่าน |
| ค้นคว้าหลังเรียน (REQ-03) | 4 ข้อต่อบทและคำค้นอย่างน้อย 2 คำต่อข้อ | ผ่าน |
| Quiz (REQ-04) | 10 ข้อต่อบท, ตัวเลือก ก/ข/ค/ง, ความยาก 3/4/3, กระจายตำแหน่งคำตอบ | ผ่าน |
| ไฟล์ภาพ (REQ-07) | ภาพ 37 ไฟล์ มีแหล่งที่มาใน [IMAGE_SOURCES](IMAGE_SOURCES.md); SVG parse ได้ | ผ่าน |
| ลิงก์ (NFR-04) | README รากและ README ของทั้ง 7 บท | ผ่านตามวิธีด้านล่าง |
| ไฟล์ที่ห้ามเผยแพร่ | ตรวจ git tracked files/history และ ignore rules สำหรับ answer-keys, _reference, secrets.h | ไม่พบใน Git |

LiquidCrystal I2C 1.1.2 ประกาศ architecture เป็น avr จึงมีคำเตือน metadata เมื่อใช้ ESP32; บาง build แสดงคำเตือน binary constants แบบเก่าจาก source ของ library ภายนอก โค้ดแล็บ compile ผ่าน ไม่ได้เปลี่ยน library ต้นทาง

## วิธีทำซ้ำ

รันจากโฟลเดอร์ repository โดยมี Arduino CLI/core/library ตามรายการด้านบน:

```powershell
./scripts/validate-repository.ps1
./scripts/validate-quizzes.ps1
arduino-cli compile --fqbn esp32:esp32:esp32 --warnings all labs/lab07-smart-home-project/code/lab07_smart_home
npx -y markdown-link-check -q -c .markdown-link-check.json labs/lab07-smart-home-project/README.md
```

เปลี่ยน path สุดท้ายของคำสั่ง compile เป็นโฟลเดอร์ sketch ที่ต้องการทดสอบ และรัน link check ซ้ำกับ README ราก/ทุกบท ในการตรวจรอบนี้ compile แบบเดียวกันครบทั้ง 30 sketches

`validate-quizzes.ps1` ต้องมี `answer-keys/` ในเครื่องผู้สอนเพื่อเทียบคำตอบและตรวจการกระจายตำแหน่ง โฟลเดอร์นี้ถูก ignore ตามข้อกำหนด จึงไม่มีใน clone จาก GitHub โดยตั้งใจ ผู้เรียนยังทำแบบทดสอบได้จาก README

Link checker ยกเว้น URL แบบเจาะจง 3 รายการใน `.markdown-link-check.json` ที่มี JavaScript หรือปฏิเสธ automated request: Silicon Labs CP210x driver, WCH CH341SER และ MQTTX Web ตรวจหน้าเว็บทางการเพิ่มเติมแล้ว หน้า WCH/MQTTX ส่งหน้าเริ่มต้นที่ต้องใช้ JavaScript การตรวจนี้ไม่ครอบคลุมการดาวน์โหลด driver หรือการ Connect broker จาก browser จริง

## ขอบเขตที่ยังต้องตรวจบนอุปกรณ์จริง

**อัปเดต Lab07:** มีผลตรวจ [Samsung S25 Ultra + ESP32-S3 จริง](../labs/lab07-smart-home-project/app-blueprint/mobile/PHYSICAL_TEST_RESULTS.md) เพิ่มในวันเดียวกันแล้ว ได้แก่ upload/verify, MQTT/reported GPIO, invalid sensorเมื่อไม่มีโมดูล และ lifecycle/faultของแอป1.0.1 ข้อความด้านล่างอธิบายขอบเขต QAคอร์สในรอบเดิม ไม่ใช้ผลบอร์ดเปล่ารับรอง sensor/วงจรโหลด

ผลด้านบนเป็นการตรวจ build และเอกสาร **ไม่ได้ upload โปรแกรมหรือยืนยันวงจรจริง** ภาพจำลอง/expected logs ไม่ใช่ผลวัด hardware ใช้ใบงานในแต่ละบทบันทึกผลจริง โดยเฉพาะ:

- ระดับแรงดัน input ของ Relay, pull-up/level shifter, ไฟเลี้ยง และการกระตุกช่วง boot
- OLED address/controller, DHT11 valid/invalid และอายุข้อมูลจริง
- MQTT สองทิศทางจาก ESP32 ↔ MQTTX ตาม REQ-08, reconnect และ LWT บนเครือข่ายที่ใช้งาน
- Android App ↔ Relay ↔ reported state รวมกรณี offline/Reset และ sensor fault

Lab07 รายงาน state ที่ firmware สั่ง GPIO ไม่มี sensor ยืนยันหน้าสัมผัส Relay หรือโหลดจริง ตัวอย่างคง Relay state เมื่อ network หลุดและเริ่ม OFF เมื่อ Reset สำหรับ **โหลด LED DC สาธิต** ต้องประเมินนโยบายใหม่ก่อนนำไปใช้กับโหลดอื่น

ห้ามใช้ AC 220V และห้ามส่งข้อมูลส่วนตัว/รหัสผ่านบน public broker รายละเอียดการทดสอบและช่องบันทึกหลักฐานอยู่ใน README ของ [Lab05](../labs/lab05-relay-ultrasonic/README.md), [Lab06](../labs/lab06-wifi-mqtt/README.md) และ [Lab07](../labs/lab07-smart-home-project/README.md)

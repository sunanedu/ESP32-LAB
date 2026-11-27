# ผลตรวจ Samsung S25 Ultra และ ESP32-S3 จริง

วันที่ **26 พฤศจิกายน 2026 (Asia/Bangkok)** ติดตั้ง APK standalone ผ่าน USB และ upload sketch ไปยังบอร์ดจริง ผู้สอนยืนยันว่าต่อ **เฉพาะ ESP32 ไม่มี OLED, DHT11, Relay หรือโหลด** จึงแยกผลสื่อสาร/reported GPIO ออกจากผลวงจร

## รุ่นที่ตรวจ

| รายการ | ค่า |
|---|---|
| โทรศัพท์ | Samsung Galaxy S25 Ultra, SM-S938B, Android17 / API37, arm64 |
| หน้าจอ | 1440×3120, density600; ไม่เปลี่ยน resolution/font ของผู้ใช้ |
| แอป | com.esp32lab.smarthome, 1.0.1 / versionCode2, standalone Release ใช้ test signing key |
| ชิปจาก esptool | ESP32-S3 QFN56 revision0.2, Flash16MB, embedded PSRAM8MB |
| พอร์ต | USB-to-UART CH343, COM7; ไม่ใช่ native USB CDC |
| Toolchain | Arduino CLI1.6.0 จาก Arduino IDE, Arduino-ESP32 core3.3.13, esptool5.3.1 |
| MQTT | แอป/observer ผ่าน WSS8084; บอร์ด TCP1883, broker.emqx.io |
| Namespace | home/lab-s25-b884f3; ข้อมูลสมมติของรอบตรวจนี้เท่านั้น |

APK ขนาด **43,164,520 byte**, SHA256:

```text
433F0B805F2F1F5967518C41D46F4E8AE9416F7F58468501D57614C03D858CF7
```

[Source และ APK 1.0.1](https://github.com/sunanedu/ESP32-LAB/raw/refs/heads/main/downloads/esp32-smart-home-preview-1.0.1.apk) ใช้ source ตาม tag นี้และ dependencies ใน lockfile ไม่มี Wi-Fi password ของบอร์ดใน APK ข้อมูล Wi-Fi อยู่ใน `secrets.h` เฉพาะเครื่องและถูก ignore

## การปรับ UI จากโทรศัพท์จริง

ผู้สอนขอขอบคมแบบ FloatingNavDemoV.2 / MinimalTodoNative จึงใช้เส้นขอบทึบ1dp `#DCE3EC`, การ์ด radius16dp, ปุ่ม/ฟอร์ม14dp และ elevation0 เอา drop shadow ออก ปุ่มที่กดเปลี่ยนสีพื้นโดยคงเส้นขอบ แก้ requirements/design/AGENTS/prompts ให้ตรงกัน ภาพ AI เดิมเป็นแบบร่างก่อนการปรับนี้

[ภาพก่อนปรับ](../evidence/lab07-s25-before-ui.png) · [Dashboard](../evidence/lab07-s25-dashboard.png) · [เพิ่ม](../evidence/lab07-s25-add.png) · [แก้ไข](../evidence/lab07-s25-edit.png) · [ตั้งค่า](../evidence/lab07-s25-settings.png)

ภาพทั้งหมดจับจากโทรศัพท์จริง ไม่ผ่านการวาด/แต่งภาพ ใช้ `adb shell screencap -p` และ `adb pull` หน้าฟอร์ม/ประวัติเลื่อนได้ จึงไม่บีบเนื้อหาทั้งหมดลงใน screenshot เดียว

## ผลตรวจแอปบน Samsung

ผลดิบพร้อมเวลา: [s25-ui.json](../evidence/s25-ui.json)

| กรณี | ผลจริง |
|---|---|
| Install / upgrade | ติดตั้ง1.0.0 แล้ว upgrade1.0.1ด้วย install-r สำเร็จ ไม่ uninstall/clear app data; profile/settings เดิมอยู่ |
| เพิ่ม/แก้ไข | เพิ่ม ESP32 Lab แล้วแก้เป็น ESP32-S3 Lab บันทึกได้; STUDENT_ID หน้า Edit อ่านอย่างเดียว |
| Settings / keyboard | Port0ถูกปฏิเสธด้วยข้อความภาษาไทย แก้8084แล้วบันทึก/reconnect WSSได้ |
| Back จาก Edit | เปลี่ยนชื่อชั่วคราวแล้วทิ้งการแก้ไข ชื่อที่บันทึกเดิมยังอยู่ |
| Back จาก Add | กรอก profile ใหม่แล้วทิ้ง ไม่เพิ่มรายการใน DB |
| Board ยังไม่ส่งข้อมูล | Brokerเชื่อมแล้ว แต่ ESP32รอยืนยัน; ปุ่มสองช่อง disabled |
| Typecheck / lint / tests | ผ่านบน source1.0.1, tests19ข้อ/3files |
| Native build | Release arm64-v8a/x86_64 ผ่านและรันบน Samsung โดยไม่ใช้ Metro |

## เฟิร์มแวร์และ pin mapping

S3 ใช้ขาแตกต่างจาก ESP32 รุ่นเดิม: DHT4, Relay16/17, OLED8/9 เลือกจาก target ใน config/ sketch ไม่ใช้ GPIO26/27 ที่สงวนกับหน่วยความจำ และไม่ใช้ GPIO22 ซึ่งไม่มีบน S3 อ่าน [ขั้นตอนและโค้ดเต็มของ Lab07](../../README.md) ก่อนต่อโมดูล

ก่อน upload สำรอง Flash เดิม16MB ไว้เฉพาะเครื่องใน `_reference/device-test/` ไม่เผยแพร่ binary หรือ credentials ใน GitHub

| Sketch / กรณี | ผลจริง |
|---|---|
| lab01_chip_info | Upload/verify hashผ่าน; Serialอ่าน ESP32-S3,2cores,240MHz,Flash16MB |
| lab07_oled_test | Upload/verify hashผ่าน; Serialแจ้ง no I2C ACK at0x3Cตามสภาพไม่มีจอ |
| lab07_dht11_test | Upload/verify hashผ่าน; Serialแจ้ง NaNตามสภาพไม่มี sensor |
| lab07_smart_home | Upload/verify hashผ่าน; บอร์ดต่อ Wi-Fi/MQTT, แอปรับ state/invalid sensor และสั่งสองช่อง/ปิดทั้งหมดได้ |

Compile ทั้ง3sketchesของ Lab07 ผ่านทั้ง ESP32 รุ่นเดิมและ S3 รวม6buildsด้วย `--warnings all` ส่วน Lab01 compile/uploadเป็น S3 ดู [ผล build/upload ที่ตรวจจาก log จริง](../evidence/firmware-s3-build-upload.json)

## MQTT ระหว่างโทรศัพท์กับบอร์ดจริง

ผลดิบพร้อมเวลาของแต่ละกรณี: [s25-esp32-hardware.json](../evidence/s25-esp32-hardware.json)

[MQTT log จริงจาก observer](../evidence/mqtt-s25-esp32-hardware.jsonl) เก็บเฉพาะ namespace ของรอบตรวจ ไม่มีตัวจำลองส่ง sensor/state ในรอบนี้; [Serial log จากบอร์ด](../evidence/serial-s3-hardware.txt) คงข้อความจริงที่ขึ้นต้นด้วย tag โดยตัด MAC/local IP ออกก่อนเผยแพร่

| กรณี | ผลจริง |
|---|---|
| TCP→broker→WSS | แอปเห็น Brokerเชื่อมแล้ว/ESP32ออนไลน์แยกกัน รับ JSONทุกประมาณ5s |
| ไม่มี DHT11 | temp/hum/age_sเป็นnull,valid=false, แอปแสดงขีด/อ่านค่าไม่ได้ ขณะปุ่มสั่ง GPIO ยังใช้ได้ |
| ON/OFF / All Off | กดจาก Samsung ส่ง6คำสั่งตามลำดับที่ตรวจ บอร์ดรายงานกลับสองช่องตรงกับ UI; observerเห็น retain=false |
| สั่งจาก clientอื่น | MQTT observer ส่ง Relay2ON แล้วแอปแสดงONจาก stateของบอร์ดจริง |
| Payloadผิด | ส่ง BAD, Serialแจ้ง ignored command และ stateช่อง2ยังONในรอบรายงานถัดไป |
| Force-stop / foreground | profile, cacheและhistoryอยู่; observerไม่พบคำสั่ง actuatorเพิ่มจากการเปิดแอปใหม่/กลับforeground |
| Offline ในโทรศัพท์ | ตัด client แล้วปุ่ม disabled, cacheและประวัติ sensorจาก SQLiteยังอ่านได้; reconnectไม่มีคำสั่งเพิ่ม |
| ไม่ได้รับรายงาน | ค้าง EN ใน Reset ผ่าน RTS แล้วกด ON: pendingก่อน timeoutประมาณ8s, stateยังOFF และ historyแจ้งยังไม่พบรายงานที่ตรงกัน |
| LWT | ขณะค้าง Reset brokerตรวจขาด heartbeatแล้วส่ง offline; แอปแสดงออฟไลน์และปิดปุ่มทั้งคู่ ดู [ภาพจริง](../evidence/lab07-s25-offline.png) |
| กลับจาก Reset | บอร์ดต่อกลับเป็นonlineและรายงานOFFทั้งคู่; จำนวน `/set` ก่อน/หลัง reconnectเท่ากัน9ข้อความ คำสั่งONที่timeoutไม่ถูกส่งซ้ำ |

ผ่าน runtime/MQTT กับบอร์ดจริง **11กรณี** ใน JSON และ UI5กรณีใน s25-ui.json เมื่อจบการตรวจ firmware Smart Home ยังอยู่บนบอร์ด, profile ESP32-S3 Labยังอยู่ใน Samsung และทั้งสองช่องรายงานOFF คืนค่า screen stay-awakeของโทรศัพท์ตามเดิมแล้ว

ตรวจไม่พบชื่อ Wi-Fi/รหัสผ่านที่ใส่ในเครื่องใน payloadที่แตกจาก APK และไฟล์ที่จะเผยแพร่ผ่าน Git ใช้ `.gitignore` กัน secrets.h/Flash backup/โฟลเดอร์เฉลย

การค้าง EN เป็นการทดสอบ CPU reset/ขาด heartbeat ไม่ได้ถอดสาย USB หรือทดสอบไฟเลี้ยงตก ไม่เปลี่ยนเครือข่าย/ข้อมูลส่วนตัวของโทรศัพท์เพื่อจำลอง fault

FQBN สำหรับบอร์ดรอบนี้:

```text
esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi,CDCOnBoot=default,UploadSpeed=115200
```

ใช้ upload921600 ผ่าน CH343 และอ่าน Serial115200; ค่า upload speed ไม่ใช่ baud ของ Serial Monitor ขนาด Flash/PSRAM ต้องตรวจบอร์ดจริงก่อนใช้กับเครื่องอื่น

## ขอบเขตที่ยังต้องต่ออุปกรณ์

- ยังไม่วัดอุณหภูมิ/ความชื้นจริง ไม่ทดสอบ OLED แสดงผลหรือ address ของจอจริง
- ไม่มี Relay/โหลด จึงไม่ยืนยันแรงดัน GPIO หน้าสัมผัส การติดของ LED หรือการกระตุกตอนเปิดเครื่อง
- ไม่ใช้ไฟ AC220V ผล `/state` หมายถึง firmware รายงานค่าที่สั่ง GPIO
- ไม่ได้เปิด MQTTX GUI ใช้ MQTT observer แยก client เพื่อเก็บหลักฐาน public broker ใน namespace ของตน
- TalkBack, Androidหลายยี่ห้อ และ endurance ระยะยาวยังไม่ทดสอบ

ผล emulator/ตัวจำลองก่อนหน้านี้เก็บแยกใน [TEST_RESULTS.md](TEST_RESULTS.md) ไม่เปลี่ยนผลจำลองให้กลายเป็นผล sensor/Relay จริง

## เทียบรายการ H01–H12 ของ test-plan

11กรณีที่ผ่านด้านบนเป็นกรณี runtimeที่รันจริง ไม่หมายถึงผ่าน hardware checklistทุกข้อครบทั้งวงจร

| ID | สถานะและขอบเขตจริง |
|---|---|
| H01 | Upload/subscribe/telemetryผ่าน ใช้ observerแทน MQTTX GUI; ไม่มี DHT11ให้อ่านค่าปกติ |
| H02 | ผ่าน: Samsung WSS, broker/presenceแยกกัน และรอข้อมูลสดก่อนสั่ง |
| H03 | ผ่านทิศทางคำสั่ง/Serial/state/UI; ยังไม่ตรวจโหลดจริง |
| H04 | ผ่านด้วย MQTT clientแยกจากแอป; ยังไม่ได้เปิด MQTTX GUI |
| H05 | ผ่าน OFFสองtopicและ reportedทั้งสองช่อง; ยังไม่วัดหน้าสัมผัส |
| H06 | ยังไม่ทดสอบการปิด Wi-Fiของโทรศัพท์ขณะpending; ตรวจ timeoutเมื่อบอร์ดค้างReset และ manual clientdisconnectแยกไว้แล้ว |
| H07 | ตรวจ LWT/stale/offlineด้วย ENค้างResetผ่าน; ยังไม่ทดลองถอดไฟUSB/ไฟตก |
| H08 | ผ่านการปล่อยReset: reconnectและOFFทั้งคู่; ไม่ยืนยันช่วงไฟเลี้ยง/หน้าสัมผัสจริง |
| H09 | ตรวจ JSONnull/validfalseเมื่อไม่มีDHTผ่าน; ยังไม่ทดลองถอดDATAหลังเคยอ่านค่าดี |
| H10 | ผ่าน force-stop/persistence/cache/history และไม่ส่ง actuatorซ้ำ |
| H11 | แอปกับบอร์ดทำงานโดยไม่เปิด MQTTX; observerเป็นเพียงclientเก็บหลักฐาน |
| H12 | Retention/clear/deleteตรวจด้วย automated SQLite/controller tests; รอบSamsungตรวจ persistence/offline history แต่ยังไม่ล้างประวัติ/ลบprofileที่คงไว้ให้ผู้สอน |

หลังเผยแพร่ตรวจ digest ของ APK บน GitHubตรงกับ SHA256ด้านบน และ link checkerผ่าน READMEราก/Lab07/Blueprint/Mobile/รายงานนี้ โฟลเดอร์เฉลย ข้อมูล Wi-Fi และ Flash backupไม่อยู่ใน commit

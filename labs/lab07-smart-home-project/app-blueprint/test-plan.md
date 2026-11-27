# Test Plan — ตรวจแอปที่ผู้เรียนสร้างภายหลัง

เอกสารนี้เป็นแผนทดสอบ **โปรเจกต์ใหม่ของผู้เรียน** ที่สร้างตามระบบอ้างอิง1.0.1 ผู้สอนมีผลจริงแล้วแต่ไม่นับแทนงานใหม่ ให้สร้างTEST_RESULTS.mdของตน ระบุวันที่/รุ่นAndroid/เครื่อง/Expo/dependencies,sourcecommit,APKversion/hash/ประเภทkey,คำสั่งและผลจริง กรณีไม่ได้รันใช้ “ยังไม่ทดสอบ”

## A. ตรวจในเครื่องด้วย unit/integration tests

| ID / APP | วิธีทดสอบ | เกณฑ์ผ่าน |
|---|---|---|
| T01 / 02 | Launch กับ DB ว่าง | Dashboard + เพิ่มอุปกรณ์ ไม่มี login/หน้าบังคับตั้งค่า |
| T02 / 03 | Viewport360dp, font scaling, screen reader labels | ไม่ล้น, scroll ได้, touch≥48dp, icons มีชื่อ |
| T03 / 05 | Add/Edit/Cancel/duplicate/IDผิด/max5 | บันทึกเฉพาะค่าผ่าน validate, Cancel ไม่เขียน |
| T04 / 06 | repository/sql parameters/migration/DB error | ข้อมูลไม่หาย, error ไม่ถูกแสดงเป็นสำเร็จ |
| T05 / 06 | samples>10,000/เกิน7วัน + commands/events เกินเพดาน | prune ต่อ device ตามนโยบาย ไม่ลบ profiles |
| T06 / 07 | valid JSON, null/valid=false, malformed, wrong types, topicอื่น, payloadเกิน | parser ไม่ล่ม ไม่ปลอม 0 ไม่เขียนค่าผิด |
| T07 / 07 | retained online/state ใน session ใหม่ | แสดง cached/รอยืนยัน ไม่เปิด controlsจาก retained อย่างเดียว |
| T08 / 07 | live packetแล้วเดินเวลา15s + offline status | สดแล้ว stale/offline และ controls disabled ตามจริง |
| T09 / 08 | กดON→publish callbackสำเร็จแต่ไม่ส่งstate | ยัง pending แล้ว timeout8s ไม่รายงาน GPIO ON ล่วงหน้า |
| T10 / 08 | stateผิดค่า/ตรงค่าหลังส่ง/packetก่อนส่ง | matchเฉพาะรายงานที่มาหลังส่งและตรงค่า; ที่เหลือรอตามนโยบาย |
| T11 / 08 | ปิดทั้งหมดแล้วช่องหนึ่งรายงาน/อีกช่อง timeout | สอง outcome ไม่อ้างว่าทั้งคู่สำเร็จ |
| T12 / 09 | disconnect/backgroundขณะpending + reconnect | interrupted และไม่มี offline queue/replay |
| T13 / 09 | mount/render/foregroundซ้ำหลายครั้ง | client/listenersไม่เพิ่มซ้ำ subscriptionไม่ซ้อน |
| T14 / 05/06 | ลบprofile/ล้างประวัติด้วยmock MQTT spy | DBเปลี่ยนตามนโยบาย ไม่มี publish actuator |

ใช้ fake timers/fixtures กับ parser/state machine ให้ทำซ้ำได้ ผลจากส่วนนี้ไม่ใช่ผลทดสอบกับ Relay จริง

## B. ตรวจ runtime บน Android

1. จดวิธีรันจริงและสร้างstandaloneRelease APKบนAndroid ปิดMetroแล้วเปิดจากlauncher ต้องโหลดJS/Hermes/fontsได้ Expo Go/development buildช่วยพัฒนาได้แต่ไม่แทนacceptanceAPKนี้
2. เพิ่ม profile ขณะไม่มีเน็ต แก้ชื่อ/ไอคอน/Relay labels แล้ว force-stop/เปิดใหม่ ต้องคงค่า
3. เปิด history จาก DB แบบ offline ได้; ช่องค่าปัจจุบันติดป้าย cached/รอยืนยันและปุ่มควบคุม disabled
4. ทดสอบ keyboard ไม่บัง Save, ปุ่ม Back/Cancel, unsaved change dialog และการยืนยันลบ
5. ตั้ง port/path ผิด บันทึกแล้วเห็น error ที่แก้ได้ เปลี่ยนคืน 8084 /mqtt แล้วเชื่อมได้
6. ทดสอบ background/foreground และเปิดแอปใหม่ ต้องไม่ส่ง ON/OFF อัตโนมัติ และไม่แสดง pending เก่าเป็นคำสั่งที่จะส่งต่อ
7. เปิด font scaling และทดลองหน้าจอเล็กทั้ง 4 หน้า จับภาพที่อ่าน label/value ได้

## C. End-to-End กับ ESP32 และ MQTTX

ใช้งานเฉพาะโหลด LED DC แรงดันต่ำ ห้าม AC220V ใช้ STUDENT_ID สมมติของกลุ่มตนตั้งใน firmware และ profile ให้ตรง ก่อนเริ่มล้าง retained command เฉพาะ topic ตนถ้าเคยมี

| ID | ขั้นตอน | ผลที่ต้องเห็น / หลักฐาน |
|---|---|---|
| H01 | Upload firmwareตามFW-01–10และimportMQTTX6 exact topicsตามMX-01–06 | status online, sensorประมาณ5s, stateทั้งสองช่อง; ไม่เปิดhome/id/#ซ้อน |
| H02 | แอปต่อ WSS, รอข้อมูล live | Brokerconnected แยกจากESP32online, controlsพร้อมเมื่อมีหลักฐานสด |
| H03 | กดRelay1 ON/OFFจากแอป | MQTTXเห็นset, Serialรับ, stateกลับ, UIreportedตรง, สังเกตโหลด |
| H04 | สั่งRelay2จากMQTTXโดยไม่กดในแอป | แอปรับstateและเปลี่ยนreportedจริง |
| H05 | สั่งปิดทั้งหมด | set OFF สองtopic และstateแยกสองช่อง |
| H06 | ปิดWi-Fiโทรศัพท์ขณะpending | UIแจ้งไม่สำเร็จ/ขาดการเชื่อม, คืนเน็ตไม่มีcommandเก่าถูกส่ง |
| H07 | ถอดไฟESP32ขณะMQTTXยังต่อ | LWT offlineหลังbrokerตรวจพบ, แอปstale/offlineไม่แค่เชื่อretainedonline |
| H08 | เปิดESP32กลับ | reconnect/subscribe, รอสด, RelayOFFหลังResetตามfirmware |
| H09 | ถอดไฟก่อนถอดDATA DHTแล้วเปิดใหม่ | JSON null/validfalse, แอปอ่านค่าไม่ได้แต่แยกbrokerconnectionได้ |
| H10 | ปิดแอป/force-stop/เปิดใหม่ | localprofiles/historyอยู่, cachedติดป้าย, ไม่มีคำสั่งactuatorส่งเอง |
| H11 | ปิดMQTTXแล้วใช้งานแอปกับบอร์ด | ระบบยังทำงานเพราะbrokerเป็นตัวกลาง |
| H12 | ตรวจlocalretention/ล้างประวัติ/ลบprofile | ข้อมูลถูกลบตามที่แจ้งและRelayไม่เปลี่ยนจากการลบ |

หากไม่มี hardware ให้ทำ A/B และระบุ H01–H12 ว่ายังไม่ทดสอบ ห้ามสร้าง log หรือภาพจำลองมาใส่เป็นหลักฐานผ่านจริง

ถ้ามีเฉพาะบอร์ด ให้แยกผลprotocol/GPIOที่ผ่านกับDHT/OLED/โหลดจริงที่ยังไม่ทดสอบ H03/H05ที่ไม่มีโหลดวัดเป็นผ่านเฉพาะคำสั่ง/รายงาน หากค้างENเพื่อทดสอบLWT ให้ระบุว่าเป็นResetไม่ใช่ถอดไฟ USB; หากใช้MQTT clientscriptแทนMQTTX GUIให้ระบุclientจริง ไม่ทำทุกHเป็นผ่านโดยอัตโนมัติ

## D. ตรวจสร้างตามรุ่นอ้างอิง

| ID | เกณฑ์ตรวจในโปรเจกต์ผู้เรียน |
|---|---|
| R01 | บันทึกbaseline/lockfileจริง; หากเปลี่ยนรุ่นมีcompatibility/testหลักฐาน ไม่ใช้คำว่าlatestแทนเลขรุ่น |
| R02 | เทียบ4หน้ากับassetsภาพจริง: ขอบ1dp/radius16/elevation0, font/spacing/iconfont/pressed; รายงานvalid/invalidโดยไม่hard-codeค่าในภาพ |
| R03 | Windows nativebuildจากpathสั้น/CMake3.31.6/Ninja1.12.1/OBJECT_PATH_MAX240สำเร็จ; ไม่ลบ/ย้ายsourceเดิมเพื่อแก้build |
| R04 | APKstandaloneติดตั้งบนarm64หรือemulatorx86_64 ปิดMetroได้; upgradeที่package/keyตรงกันคงprofile/history; signatureไม่ตรงไม่uninstallเอง |
| R05 | ตรวจBOARD_TARGETก่อนupload ESP32/S3mappingถูกต้อง firmwaremissingOLEDcontinueและDHTinvalidไม่ปลอม0; compileผ่านcore3.x |
| R06 | MQTTXมี6aliases/4สีตามMX,studentIDตรง3ฝั่ง,clientIDไม่ซ้ำ,Plaintextหน้าAllไม่มีSyntaxError; filterผ่านพื้นที่QoS0และclearได้ |
| R07 | คัดลอกเฉพาะ9MD+assetsไปโฟลเดอร์ใหม่แล้วอ่านได้ ภาพจริง/template/ลิงก์หลักอยู่ครบ; ไม่อ้างD:\AIส่วนตัวเป็นdependency |
| R08 | TEST_RESULTSแยกmock/emulator/Samsung/ESP32จริงและbare-board/full-circuit ระบุไม่มีphone/status,ไม่มีcausalACKและไม่มีbackgroundcollectionรับประกัน |

## ขอบเขตผลอ้างอิงที่นำมาเทียบ

- แอป1.0.1: unit/integration19ข้อ; SamsungUI5กรณี; Samsung↔ESP32-S3จริง11กรณีของprotocol/lifecycle บอร์ดไม่มีโมดูล จึงไม่ยืนยันค่าจริง/จอ/หน้าสัมผัส
- FirmwareLab07สามsketches compileผ่านESP32และS3หกรายการ core3.3.13; upload/verifyจริงสี่sketchesรวมchip-infoจากLab01
- MQTTXDesktop1.13.3: import/aliases/colors/connect/online/heartbeat/state/filterผ่าน; รอบตั้งค่ามอนิเตอร์ไม่ได้กดOFFจากโทรศัพท์เพราะล็อกหน้าจอ ไม่สร้างผลคำสั่งใหม่ในรอบนั้น
- ผลตัวจำลอง1.0.0/emulatorเก็บต่างหาก ไม่เปลี่ยนให้เป็นผลfull-circuitหรือผลโปรเจกต์ใหม่

อ่าน [ผลจริงและตารางH01–H12ของผู้สอน](https://github.com/sunanedu/ESP32-LAB/blob/main/labs/lab07-smart-home-project/app-blueprint/mobile/PHYSICAL_TEST_RESULTS.md) และ [ผลMQTTX](https://github.com/sunanedu/ESP32-LAB/tree/main/labs/lab07-smart-home-project/mqttx) เพื่อดูรูปแบบรายงาน ไม่คัดลอกสถานะผ่านลงTEST_RESULTSของตน

## รูปแบบ TEST_RESULTS.md ที่ต้องสร้างตอนทดสอบ

| Test ID | วัน/สภาพแวดล้อม/commit | ผลจริง | ผ่าน/ไม่ผ่าน/ยังไม่ทดสอบ | หลักฐาน/ข้อจำกัด |
|---|---|---|---|---|
| T01 | ____ | ____ | ____ | ____ |
| H03 | ____ | ____ | ____ | ____ |

เพิ่มครบทุก ID พร้อมสรุปคำสั่ง typecheck/lint/tests/build/runtime ที่รันจริง ภาพ 4 หน้า และวิดีโอส่งคำสั่งเทียบ MQTTX/Serial/โหลด ใช้ข้อมูลสมมติ ไม่มี Wi-Fi password, credentials หรือชื่อจริงในภาพ

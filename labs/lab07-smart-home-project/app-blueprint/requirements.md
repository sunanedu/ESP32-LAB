# Requirements — Smart Home Android Mini Project

ชุดสเปกสำหรับใช้ OpenCode สร้างแอป **React Native + Expo + TypeScript** ให้มีพฤติกรรมและหน้าตาตรงกับแอปอ้างอิง **1.0.1** ที่ติดตั้งบน Samsung S25 Ultra แล้ว ข้อมูลหลักอยู่ในโทรศัพท์ ไม่มี backend/cloud database ที่จำเป็นต่อการใช้งานหลัก เอกสารนี้กำหนดสิ่งที่ผู้เรียนต้องสร้างใหม่ ผลของผู้สอนไม่ใช่ผลผ่านของโปรเจกต์ผู้เรียน

## ผู้ใช้และเป้าหมาย

ผู้เรียนเปิดแอปแล้วเห็นหน้าควบคุมทันที เพิ่ม ESP32 โดยกรอกรหัสสมมติเดียวกับ STUDENT_ID ใน firmware ดูอุณหภูมิ/ความชื้น และสั่ง Relay 2 ช่องผ่าน MQTT จากนั้นปิดเปิดแอปแล้วข้อมูลอุปกรณ์และประวัติยังอยู่ในเครื่อง

## APP-01 เทคโนโลยีและขอบเขต

- สร้าง React Native ด้วย Expo และ TypeScript โค้ดแอปอยู่ใน `mobile/` ภายใต้โฟลเดอร์ที่มีไฟล์สเปกนี้
- ใช้ SQLite บนโทรศัพท์ผ่าน `expo-sqlite` เป็นแหล่งข้อมูลถาวรหลักสำหรับ settings, devices, sensor history และ command/event history
- ใช้ MQTT.js ผ่าน WSS และ icon font ผ่าน `@expo/vector-icons` เช่น Ionicons
- ใช้ baseline ที่ build/runtime ผ่านแล้วด้านล่างเป็นจุดเริ่ม บันทึกรุ่นและ lockfile หากเปลี่ยนรุ่นต้องตรวจ compatibility จากเอกสารทางการและทดสอบใหม่ ไม่อัปเกรดทั้งหมดเป็น latest ระหว่าง scaffold โดยเงียบๆ
- เป้าหมายส่งงานคือรันบน Android จริงหรือ emulator พร้อมหลักฐาน build/runtime; การเผยแพร่ Play Store อยู่นอกขอบเขต

**ตรวจรับ:** มี source React Native, dependency lockfile, วิธีรันที่ทำซ้ำได้ และบันทึกเวอร์ชันจริง

### Baseline ที่ใช้สร้าง 1.0.1

| ส่วน | รุ่นที่ resolve ใน lockfile / environment ที่ตรวจจริง |
|---|---|
| Node / npm / Java | 24.19.0 / 11.17.0 / JDK17 |
| Expo / React / React Native | 58.0.26 / 19.3.0 / 0.88.1 |
| TypeScript / MQTT.js | 6.0.3 / 5.16.1 |
| expo-sqlite / crypto / font | 58.0.18 / 58.0.8 / 58.0.12 |
| expo-clipboard / status-bar | 58.0.4 / 58.0.5 |
| @expo/vector-icons / safe-area-context | 15.1.1 / 5.11.1 |
| ESLint / Vitest | 10.15.0 / 5.1.2 |
| Android build | compileSdk37, targetSdk36, minSdk24, Gradle9.4.1, NDK27.1.12297006 |
| Windows CMake / Ninja | 3.31.6 / 1.12.1; native buildในpathสั้น, CMAKE_OBJECT_PATH_MAX=240 |
| Runtime ที่ตรวจ | Samsung S25 Ultra Android17/API37 arm64; emulator Android16/API36 x86_64 |

ตัวเลขนี้เป็นผล environment ของผู้สอน ไม่ใช่คำรับรองว่าทุกรุ่นใหม่เข้ากัน และไม่ใช่รุ่นขั้นต่ำทั้งหมด ใช้ package-lock ของแอปอ้างอิงเมื่อเลือกเส้นทางรัน source เดิม ดูคำสั่งใน [README.md](README.md)

## APP-02 หน้าจอ 4 หน้า

| หน้า | ต้องมี |
|---|---|
| Dashboard (หน้าแรก) | ชื่อ Smart Home, ปุ่มเพิ่ม/ตั้งค่า, เลือกอุปกรณ์, สถานะ broker/ESP32 แยกกัน, temperature/humidity, Relay 1/2, ปุ่มปิดทั้งสองช่อง, ประวัติล่าสุด |
| Add Device | ชื่ออุปกรณ์, ห้อง/พื้นที่, STUDENT_ID, เลือกไอคอน, แสดง prefix ที่คำนวณได้, บันทึก/ยกเลิก |
| Edit Device | ชื่อ/ห้อง/ไอคอน/ชื่อ Relay แต่ละช่อง, ID และ prefix อ่านอย่างเดียว, บันทึก/ยกเลิก, ลบออกจากโทรศัพท์พร้อมยืนยัน |
| Settings | host, WSS port, path, เชื่อมอัตโนมัติ, บันทึกและเชื่อมต่อ, สถานะเชื่อมต่อ, ข้อมูล local storage และล้างประวัติพร้อมยืนยัน |

เปิดแอปต้องเข้า Dashboard ไม่มี login, welcome carousel หรือหน้าบังคับตั้งค่าก่อน ถ้ายังไม่มีอุปกรณ์ให้แสดง empty state กับปุ่ม “เพิ่มอุปกรณ์” ใน Dashboard; ตอนอ่าน DB แสดง loading indicator ในหน้าเดิม ปุ่มเพิ่ม/ตั้งค่ารอ DB พร้อมก่อนใช้งาน ปุ่มที่ยังใช้ไม่ได้ต้อง disabled พร้อมเหตุผล

**ตรวจรับ:** navigation ทุกเส้นทางกลับ Dashboard ได้ Android Back ไม่ทำให้ข้อมูลที่ยังไม่บันทึกถูกบันทึกเอง

## APP-03 รูปลักษณ์

พื้นหลังขาว การ์ดขาว ขอบเทาอ่อนทึบ 1 dp มุมโค้ง 16 dp ไม่มีเงาฟุ้งหรือ elevation รอบการ์ด/ปุ่ม ตามการตรวจบนโทรศัพท์จริงของผู้สอน สีข้อความเข้ม ใช้สีฟ้าเป็น accent ปุ่มคำสั่งเปิดและสถานะสำเร็จแยกความหมายด้วยข้อความ/ไอคอน ไม่ใช้สีอย่างเดียว ขนาดกดขั้นต่ำ 48×48 dp รายละเอียด spacing/type/สีใน [design.md](design.md)

ใช้ icon font ไม่ใช้ emoji เป็นไอคอนหลัก สไตล์เรียบสะอาดที่ได้แรงบันดาลใจจาก Home dashboard ทั่วไป ไม่คัดลอกโลโก้/เครื่องหมายการค้าของแบรนด์ใด

**ตรวจรับ:** หน้าจอ 360 dp ไม่ล้นแนวนอน ฟอนต์ระบบขยายแล้วยังอ่าน/กดได้ มี accessibility label ของปุ่มไอคอน

## APP-04 Broker และ topic

- Public broker ฟรีของ EMQX: `broker.emqx.io`; MQTTX เป็น client สำหรับตรวจข้อความ
- แอป React Native และ MQTTX Web: `wss://broker.emqx.io:8084/mqtt`; MQTTX Desktop monitor ใช้ TCP1883/MQTT3.1.1 ตามค่าที่ตรวจจริง
- ESP32 firmware: `broker.emqx.io:1883` (TCP) ตามแล็บ ไม่มีการเปลี่ยน firmware มาใช้ WSS
- MQTT 3.1.1, QoS 0, command retain=false; subscribe เฉพาะ topic ของอุปกรณ์ที่บันทึกไว้
- Namespace ต่อบอร์ด: `home/<student_id>` ใช้สัญญาเดียวกับ [mqtt-contract.md](mqtt-contract.md)
- ไม่ต้องเปิด MQTTX ทิ้งไว้ แอปกับ ESP32 ติดต่อผ่าน broker ได้เอง ใช้ Client ID ต่างกันทุก client
- แอป subscribe4หัวข้อต่อบอร์ด (sensor/status/state1/state2) ส่วน MQTTX monitor subscribe6หัวข้อ (เพิ่มset1/set2) พร้อม alias/สีตาม mqtt-contract.md; ห้ามนำsetที่เห็นมาปลอมเป็นreported

**ตรวจรับ:** ทดสอบ App → set → ESP32 → state → App และ MQTTX → set → App เห็น state ได้สอง client

## APP-05 เพิ่ม/แก้ไข/ลบอุปกรณ์ในเครื่อง

รองรับไม่เกิน 5 บอร์ดในเครื่อง แต่เริ่มทดสอบจาก 1 บอร์ด แต่ละบอร์ดมี 2 Relay และ DHT11 หนึ่งตัว STUDENT_ID ยาว 1–24 ASCII ตาม `[A-Za-z0-9-]+` ไม่ซ้ำกับอุปกรณ์ในเครื่อง และตรงกับ firmware ตัวพิมพ์เล็ก/ใหญ่ทุกตัว

Add เป็นการบันทึก profile/topic เท่านั้น ไม่ flash firmware, provision Wi-Fi หรือค้นหา Bluetooth อัตโนมัติ Edit ไม่เปลี่ยน STUDENT_ID เพื่อกันย้าย namespace ผิดบอร์ด ถ้าต้องเปลี่ยนให้เพิ่ม profile ใหม่หลังแก้ firmware ให้ตรงกัน

ลบ profile ต้องยืนยัน หยุด subscription ของอุปกรณ์นั้น ลบข้อมูล local ของอุปกรณ์นั้นตาม data model และไม่ส่งคำสั่งเปลี่ยน Relay

**ตรวจรับ:** บันทึกแล้วปิดเปิดแอปยังอยู่, duplicate ID ถูกปฏิเสธ, Cancel ไม่เขียน DB, ลบ profile ไม่เปลี่ยนฮาร์ดแวร์

## APP-06 ข้อมูลถาวรในโทรศัพท์

เก็บ broker settings, installation ID สุ่ม, profiles/labels/icons, sensor samples, command/events และ cache ล่าสุดใน SQLite พร้อม schema version และ migration ตาม [data-model.md](data-model.md) อ่าน DB ได้แม้ไม่มีอินเทอร์เน็ต ใช้ parameterized SQL และ transaction กับการเปลี่ยนข้อมูลที่เกี่ยวข้อง

เก็บ sensor ไม่เกิน **10,000 แถวต่อบอร์ดและไม่เกิน 7 วัน** (ลบตามขีดจำกัดที่ถึงก่อน ไม่รับประกันว่ามีข้อมูลครบ 7 วัน) commands ไม่เกิน 1,000 แถวต่อบอร์ด และ events ไม่เกิน 500 แถวต่อบอร์ด ล้างประวัติไม่ลบ profiles/settings เว้นแต่ผู้ใช้เลือกลบ profile

**ตรวจรับ:** force-stop/เปิดใหม่คืนข้อมูลได้, mock เวลา/ข้อมูลจำนวนมากตรวจ retention, DB error แสดงข้อความและไม่ล้าง DB ทิ้งเงียบๆ

## APP-07 ความถูกต้องของการ monitor

แยกสถานะ **โทรศัพท์ต่อ broker** ออกจาก **ความสดของ ESP32** การเห็น retained online อย่างเดียวไม่ยืนยันว่าบอร์ดยังออนไลน์ ใช้ข้อมูล live ของ sensor/state และเวลาได้รับล่าสุดประกอบ

- DB cache และ retained packet แสดงได้แต่ติดป้าย “ข้อมูลล่าสุดที่บันทึก” หรือ “รอยืนยัน”
- ถ้าไม่ได้รับ packet live ที่ตรวจ schema ผ่านภายใน 15 s ให้แสดงข้อมูลเก่า/ไม่ทราบสถานะและปิดปุ่มสั่งงาน
- ความสดsensorมีtimerของตนเอง การได้รับstateอย่างเดียวไม่ทำให้sensorเก่ากลับเป็นสด; ปิดปุ่มทันทีเมื่อstatus=offline
- รอบ sensor `valid=false`, temp/hum=null ต้องแสดง “อ่านค่าไม่ได้” ไม่แทนเป็น 0 และไม่ทำให้ค่าดีเก่าดูเหมือนค่าปัจจุบัน
- เวลาหน้าจอใช้เวลาที่โทรศัพท์ได้รับข้อความพร้อมหน่วย; uptime ของ ESP32 ไม่ใช่เวลาปฏิทิน
- malformed payload/topic แปลกไม่ทำให้แอปล่ม และไม่เขียนทับค่าดีล่าสุด

**ตรวจรับ:** retained online เก่า, Wi-Fi ขาด, NaN/null และ JSON ผิดรูปแบบแสดงผลต่างกันถูกต้อง

## APP-08 สั่งงานและ reported state

กดปุ่มส่งข้อความ ON/OFF เฉพาะ `/relay/{1|2}/set` ห้าม publish ไป `/state` ให้แอปแสดง “กำลังรอรายงาน” จนมีข้อความ state ที่ตรงกับ desired value หลังส่ง ถ้าไม่พบภายใน 8 s ให้แจ้ง “ยังไม่พบรายงานที่ตรงกัน” โดยไม่ส่งซ้ำเอง

state ที่แสดงต้องมาจาก `/state` ไม่เปลี่ยนเหมือนสำเร็จเพียงเพราะ publish callback สำเร็จ การพบ state ตรงกันเป็นหลักฐานรายงาน GPIO เท่านั้น สัญญาปัจจุบันไม่มี commandId จึงไม่รับประกัน causal acknowledgment หรือการทำงานทางกายภาพ

ปุ่ม “ปิดทั้งหมด” ส่ง OFF ให้ 2 ช่องของบอร์ดที่เลือกและแสดงผลแยกช่อง ไม่ใช่คำสั่ง atomic ปุ่มสั่งงานต้อง disabled เมื่อ offline/stale/กำลัง pending

**ตรวจรับ:** ตัดเครือข่ายหลังส่ง, state ไม่กลับ, กลับผิดค่า, ส่งซ้ำ และ client อื่นสั่ง ยังไม่แสดงความสำเร็จปลอม การmatchต้องเป็นstateไม่ใช่retained replayและรับหลังเริ่มส่งจริง ไม่ใช่เพียงหลังเริ่มเขียนcommandลงDB

## APP-09 Reconnect และ lifecycle

ใช้ MQTT client/service เดียว ไม่เพิ่ม listener ซ้ำจากการ render เมื่อกลับมา foreground ให้ reconnect/resubscribe ตามสถานะจริง ยกเลิก pending เก่าและรอข้อมูลใหม่ก่อนเปิดปุ่ม ไม่รับประกันการรับข้อมูลขณะถูก Android พักแอป

ห้าม queue/replay คำสั่ง actuator ตอน offline รวมทั้งปิด queueQoSZero ของ MQTT.js; อ่านรายละเอียดใน contract การ disconnect/reconnect ไม่ส่ง ON/OFF เอง สั่งล้างประวัติ/ลบ profile ไม่สั่งอุปกรณ์

**ตรวจรับ:** ปิดเน็ต กดคำสั่งไม่ได้ เปิดเน็ตกลับไม่มีคำสั่งเก่าถูกส่ง และ background/foreground หลายรอบไม่เกิด subscription ซ้ำ

## APP-10 Settings และข้อมูลส่วนตัว

ค่าเริ่มต้น host=broker.emqx.io, port=8084, path=/mqtt, autoConnect=true เมื่อมี profile พร้อมใช้ ปุ่ม “บันทึกและเชื่อมต่อ” ตรวจรูปแบบก่อนเขียนค่า แล้วปิด client เก่าและเชื่อมค่าใหม่

ไม่มี Wi-Fi SSID/password ของ ESP32 ในแอป ไม่ต้องมี MQTT credentials สำหรับ public endpoint นี้ หากต่อยอด private broker ให้เก็บ secret ใน secure storage แยก SQLite ไม่ปิดการตรวจ TLS certificate และไม่ใส่ข้อมูลลับใน log/export/Git

**ตรวจรับ:** port/path ผิดมี error ที่แก้ไขได้, ทดสอบ connection ไม่ได้ถูกแสดงเป็น ESP32 online และไม่มี secret ใน source/log

## APP-11 การสร้างด้วย OpenCode

ใช้ไฟล์ AGENTS.md + สเปกทั้งหมดในโฟลเดอร์นี้เป็นบริบท สั่งแยก Phase ตาม implementation.md ใช้ prompt ใน opencode-prompts.md ผู้เรียนต้องอ่านแผน ตรวจ diff และอธิบายผลทดสอบได้ ห้ามข้ามจากภาพ mockup ไปสรุปว่าแอปทำงานแล้ว

**ตรวจรับ:** มีรายการ Phase, commit/งานที่ตนตรวจ และอธิบาย flow ของหนึ่งคำสั่งกับข้อมูล local ได้

## APP-12 หลักฐานและ Definition of Done

ต้องมี typecheck, lint, unit/integration tests ที่เกี่ยวกับ parser/storage/state/timeout และผล Android runtime/end-to-end ใน test-plan.md พร้อมภาพ 4 หน้า, log ส่ง/รับ MQTT, ผล offline/lifecycle/persistence และรุ่น dependency จริง

สิ่งที่ยังไม่ได้ทดสอบระบุ **ยังไม่ทดสอบ** ห้ามใส่ผลผ่านสมมติ ใช้ `assets/lab07-reference-*.png` เป็นภาพจริงอ้างอิงดีไซน์ปัจจุบัน ภาพ `assets/lab07-app-four-screens-ai.png` เป็นแบบร่างเดิม ไม่ใช่หลักฐานของโปรเจกต์ผู้เรียน

### ขอบเขตและผลที่ต้องคาดหวัง

- ทดสอบบอร์ดเปล่าได้: firmwareไม่พบOLEDแล้วทำงานต่อ, DHTส่งnull/valid=false, แอปยังสั่งGPIOได้เมื่อบอร์ดมีข้อมูลสด ผลนี้ไม่ผ่านเกณฑ์sensor/จอ/หน้าสัมผัสจริงแทนการต่อโมดูล
- Public brokerไม่มีACL/authenticationของกลุ่ม ผู้ใช้topicเดียวกันส่งข้อความได้ ไม่ใช้รหัสส่วนตัว/พิกัด/Wi-Fi password; MQTTXไม่สามารถระบุว่าsetมาจากโทรศัพท์ใดจากprotocolนี้เพียงอย่างเดียว
- ไม่มีphone/status, ไม่มีcloud sync, background service, login, automation/hysteresisหรือOTA/provisioningในapp1.0.1 การเพิ่มprofileไม่ตั้งWi-Fiให้ESP32
- ข้อมูลมีเฉพาะในแอป/เครื่องนี้ ไม่มีการรับประกันbackupข้ามโทรศัพท์; Clear app data/uninstallอาจทำข้อมูลหาย การupgradeด้วยpackageและsigning keyเดิมเป็นคนละกรณี
- APKอ้างอิงเป็นstandaloneReleaseแต่ใช้test signing key ไม่ใช่Play Store release; งานผู้เรียนต้องเลือกpackageของตนและเก็บkeyจริงนอกGit

## แหล่งยืนยันทางเทคนิค

- [EMQX public endpoint และพอร์ต](https://www.emqx.com/en/mqtt/public-mqtt5-broker)
- [Expo — Create a project](https://docs.expo.dev/get-started/create-a-project/)
- [Expo SQLite](https://docs.expo.dev/versions/latest/sdk/sqlite/)
- [MQTT.js / React Native](https://github.com/mqttjs/MQTT.js)

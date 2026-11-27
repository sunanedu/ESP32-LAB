# การตั้งค่า S3 HomeGuard

> ฉบับสาธารณะ: เอกสารอธิบายแบบและผลอ้างอิงสำหรับผู้เรียนสร้างเอง source แอป/secure runtime สำเร็จเก็บแยกสำหรับผู้สอน ไม่ได้อยู่ใน repository นี้

อัปเดต 27 พฤศจิกายน 2569 · [สารบัญ](README.md) · [สร้าง APK](BUILD_GUIDE.md) · [ใช้งาน](USER_GUIDE.md)

## 1. กำหนดค่าที่ใด

| ค่าที่ต้องตัดสินใจ | ตำแหน่ง | ข้อสังเกต |
|---|---|---|
| ชื่อแอป / slug / package / versionCode | `mobile/app.json` | baseline `S3 HomeGuard` / `com.esp32lab.homeguard`; package ใหม่สร้างคลังข้อมูลและเจ้าของอีกชุด |
| เวอร์ชัน npm/dependencies | `mobile/package.json` และ `package-lock.json` | ใช้ lockfile เดียวกัน เพิ่ม version metadata ให้ตรงเมื่อออกรุ่น |
| Native SDK/Protobuf/HiveMQ | local module `android/build.gradle` และ `vendor/` | เปลี่ยนรุ่นต้องตรวจ JVM และ Release SRP self-test |
| Native Android ที่สร้างใหม่ | config plugins และ local module | ข้อแก้ต้องอยู่ใน source ก่อน Prebuild |
| สภาพบอร์ด / โมดูล / active level | `code/lab09_s3_homeguard/secrets.h` เฉพาะเครื่อง | คัดลอกจาก `secrets.example.h`; ไม่ commit ไฟล์จริง |
| Wi-Fi สำหรับบอร์ด | ฟอร์ม **เพิ่ม**; QA fallback อยู่เฉพาะเครื่อง | เมื่อจับคู่แล้วใช้ config ที่บันทึกบนบอร์ด |
| Broker ของโทรศัพท์ | **ตั้งค่า → MQTT Broker** | endpoint/transport/CA และบัญชีโทรศัพท์ |
| Broker credential ของ ESP | ฟอร์มจับคู่/ส่งโปรไฟล์ใหม่ | Private mode ต้องใช้บัญชีแยกจากโทรศัพท์ |
| UUID | อ่าน `identity` หรือรหัสท้องถิ่นจากบอร์ด | ไม่ใช้ STUDENT_ID/ชื่อบอร์ดแทน UUID |

## 2. บอร์ดและพิน

ใช้ Arduino-ESP32 **3.3.13** ของ baseline, ESP32S3 Dev Module, Flash 16MB และ PSRAM OPI **เมื่อบอร์ดจริงตรงรุ่นนี้** ชุด CH343 ผู้สอนใช้ USB CDC On Boot Disabled และ Serial/Upload 115200

เลือก Partition Scheme **16M Flash (3MB APP/9.9MB FATFS)** เพื่อขนาดแอปตามชุดผู้สอน และคง `partitions.csv` ที่อยู่กับ development sketch: app0 อยู่ `0x10000`, NVS ของ runtime อยู่ `0x610000` อย่าเปลี่ยน partition หรือ flash โดยไม่ทราบรุ่น/แผนสำรองและผลต่อ config เดิม

| อุปกรณ์ | GPIO ของ S3 |
|---|---|
| Relay 1 / 2 | 16 / 17 |
| DHT11 DATA | 4 |
| SH1106G SDA / SCL | 8 / 9 |
| โมดูลปุ่ม OUT | 18 |
| BOOT สำหรับ bare-board mode | 0 เฉพาะกรณีเปิด `HG_BARE_BOARD` |

แรงดันและสายครบดู [PINOUT](../../../assets/PINOUT.md) ไม่ย้ายพิน Lab07 เพื่อรองรับการเปลี่ยน UI

### 2.1 เลือก mode ให้ตรงของที่ต่อ

| ค่าใน secrets.h | บอร์ดเปล่า | มีโมดูลตามแล็บ |
|---|---|---|
| `HG_BARE_BOARD` | `true` ใช้ BOOT0 | `false` ใช้ OUT18 |
| `HG_WITH_DHT` | `false` | `true` เมื่อมี DHT11 ที่ต่อถูกต้อง |
| `HG_WITH_OLED` | `false` | `true` เมื่อมี SH1106G |
| `HG_RELAY_ACTIVE_LOW` | ตามค่าใช้ตรวจ GPIO; ไม่มีหน้าสัมผัสจริง | ตรวจ active level ของโมดูลก่อนใช้ |

`false` ของเซ็นเซอร์หมายถึงไม่มีการวัดจริง ต้องแสดง invalid/null ไม่สร้างอุณหภูมิให้หน้าจอดูผ่าน รหัสจาก `lab09_hardware_preflight` เป็น DEMO; ต้องใช้ `lab09_s3_homeguard` เมื่อต้องการจับคู่จริง

## 3. Broker profile

| ช่อง | Public lab | Private |
|---|---|---|
| Hostname | `broker.emqx.io` ไม่ใส่ scheme ในช่อง host | hostname ของผู้ให้บริการ |
| Transport / Port | `tls` / `8883` | TLS port ตามบริการ |
| WSS ทางเลือก | `wss` / `8084`, path `/mqtt` | WSS port/path ตามบริการ |
| Mode | `public-lab` | `private` |
| บัญชีโทรศัพท์ | เว้นว่างตาม Public profile | บัญชีเฉพาะโทรศัพท์ |
| บัญชี ESP | เว้นว่างตาม Public profile | บัญชีอีกชุด พร้อม ACL แยกทิศทาง |
| CA | ใช้ trust configuration ที่ให้มา ตรวจ certificate/hostname | CA PEM ตาม chain ของบริการ ห้ามปิดตรวจเพื่อให้ผ่าน |

ฟอร์ม **เพิ่มโปรไฟล์ broker** สร้าง profile ใหม่ การแก้โปรไฟล์ที่มีอุปกรณ์ผูกอยู่ถูกจำกัด ให้สร้าง candidate ใหม่แล้วย้ายอุปกรณ์ตาม workflow ไม่เปลี่ยน endpoint ของโทรศัพท์ฝ่ายเดียว

**ทดสอบ Broker ผ่าน** ยืนยัน TLS/MQTT ของโทรศัพท์ถึง Broker เท่านั้น ยังไม่ยืนยันว่า ESP32 ออนไลน์ ถือกุญแจเดียวกัน หรือพร้อมเปลี่ยน GPIO

## 4. ข้อมูลสำหรับ pairing

| ช่องที่ใช้ | ที่มา |
|---|---|
| ชื่ออุปกรณ์ | ป้ายที่ผู้ใช้ตั้ง ไม่ใช่ตัวพิสูจน์ตัวตน |
| Device UUID | `identity` / `[LOCAL_PAIR]` จากบอร์ด |
| BLE address / service UUID | ผลค้นหา `HG_...` ของหน้าต่างนั้น |
| Security2 username/password | อ่านท้องถิ่นในหน้าต่าง 120 s; ใช้รอบปัจจุบัน |
| AP SSID/password | ใช้เฉพาะ SoftAP แยกจาก SRP password |
| Wi-Fi SSID/password | เครือข่ายที่ให้บอร์ดเข้าถึง Broker |
| Broker profile + ESP credentials | โปรไฟล์ที่สร้างไว้และบัญชีเฉพาะบอร์ดเมื่อเป็น Private |

ตั้งล็อกหน้าจอโทรศัพท์ เปิด Bluetooth/สิทธิ์ที่ระบบร้องขอ และอ่านรหัสภายในหน้าต่างเดียวกัน เลือก transport บน firmware ผ่าน Serial `transport ble` หรือ `transport softap` แล้วกดปุ่มจริงค้าง 2 s เพื่อเปิดหน้าต่าง คำสั่ง Serial นี้ยังไม่เปิด pairing เอง

ไม่ใส่ root/session key ในฟอร์ม JS หรือไฟล์ตั้งค่าแจกผู้เรียน Native สร้าง root key เอง ไม่ใช้ SRP/Wi-Fi password เป็น root key

## 5. ค่าที่ควรทำเป็น policy และความต่างที่ต้องบันทึก

| เรื่อง | source อ้างอิงปัจจุบัน | เกณฑ์/ข้อจำกัด |
|---|---|---|
| สิทธิ์ผู้ใช้ / background | 5 นาที; background ล็อก | เปลี่ยน UX ต้องรักษา authentication |
| Deadline / pending | 5 s / รอผล 8 s | timeout คือไม่ทราบผล ให้ขอ State |
| Lease / renew | 30 s / 10 s | ปิดแอปอาจรอ Lease; ไม่รับประกัน OFF ทันที |
| QoS Telemetry | 1 ผ่าน `publishWire()` | สเปกต้นทางกำหนด 0; บันทึกเป็นความต่าง ไม่อ้างว่าตรงทุกข้อ |
| Keepalive | โทรศัพท์ 60 s; ESP 30 s | สเปกเดิมใช้ 60 s ทั้งคู่ |
| Rate / cooldown | filter 20 packets/s; client cooldown 3 failures/30 s | HG-10 ยังกำหนด command 10/s และต้องตรวจ server-side controls เพิ่ม |
| ที่เก็บ ESP | development NVS ไม่เข้ารหัส | encrypted NVS/Flash/Secure Boot เป็น production gate |

ตารางเต็มและ byte contract อยู่ใน [SYSTEM_WORKFLOW](../SYSTEM_WORKFLOW.md) ส่วนการตัดสินใจเปลี่ยนค่าต้องบันทึก [DECISIONS](../DECISIONS.md) พร้อมผลทดสอบ ไม่แก้ acceptance ให้ตรง code โดยเงียบ ๆ

## 6. เช็กลิสต์ก่อนส่งต่อให้ผู้อื่น

- [ ] ยืนยัน package/version/lockfile/commit และไฟล์ plugin ครบ
- [ ] บอร์ด รุ่น Flash/PSRAM/partition และ bare-board flags ตรงของจริง
- [ ] UUID และ transport ถูกตัว ใช้ credential จากรอบล่าสุด
- [ ] มีล็อกหน้าจอและ USB/BLE permissions ที่จำเป็น
- [ ] Broker probe ผ่าน และแยกผลจาก application authentication
- [ ] ไม่มีรหัสจริงใน Markdown/log/ภาพ/SQLite/export; `secrets.h` ถูก ignore
- [ ] ระบุ hardware ที่ไม่มี และรายการ NOT RUN ตาม [test-plan](test-plan.md)

# ผลตรวจ Lab09

> ฉบับสาธารณะ: เอกสารอธิบายแบบและผลอ้างอิงสำหรับผู้เรียนสร้างเอง source แอป/secure runtime สำเร็จเก็บแยกสำหรับผู้สอน ไม่ได้อยู่ใน repository นี้

## สถานะล่าสุดที่ควรอ่านก่อนตารางประวัติ

อัปเดตดัชนีเอกสาร 27 พฤศจิกายน 2569: มีแอป Native และ development firmware แล้ว ผลในช่วงท้ายรายงานยืนยัน BLE Security2/proof/Session และ GPIO16/17 จาก Samsung กับบอร์ดเปล่าในรุ่น2.0.5 รุ่นUIล่าสุดที่บันทึกคือ2.1.3 มีผล build/install/ห้าแท็บ ไม่ได้ทดสอบGPIOซ้ำในรอบUIนั้น

SoftAP/migration/fault/โมดูลจริง/privateACL/production gates ยังไม่ผ่านครบ ตารางเริ่มต้นด้านล่างเป็นประวัติของ starter จึงมี NOT RUN ตามเวลานั้น ห้ามใช้เป็นข้อสรุปว่าปัจจุบันยังไม่มี native runtime หรือใช้ผลBLEแทนtransportอื่น

เริ่มงานจาก [สารบัญคู่มือ](app-blueprint/README.md), ตรวจ [ปัญหาที่แก้แล้ว/ยังเปิด](app-blueprint/KNOWN_ISSUES.md) และ [ใบตรวจรับ](app-blueprint/test-plan.md) ผลการปรับคู่มือรอบนี้เป็นการตรวจเอกสาร/host ไม่สร้าง hardware PASS ใหม่

## ประวัติการตรวจตามรอบ

วันที่ 27 พฤศจิกายน 2026 (Asia/Bangkok)

## เกณฑ์รายงาน

PASS ใช้เฉพาะคำสั่งที่รันจริง แยก BUILD, HOST TEST, MOCK และ HARDWARE ไม่ใช้ภาพจำลองเป็นหลักฐาน Android/ESP32

| รายการ | สถานะเริ่มต้น | หลักฐาน |
|---|---|---|
| Arduino S3 hardware preflight | BUILD PASS | CLI1.6.0/core3.3.13 `--warnings all`; 349198 bytes ไม่มี warning |
| Arduino S3 crypto self-test build | BUILD PASS | 354894 bytes ไม่มี warning; ยังไม่รันบน MCU |
| Arduino S3 TLS broker probe build | BUILD PASS | 572206 bytes ไม่มี warning; local secrets.h ใช้ placeholder ไม่เชื่อม broker จริง |
| Python crypto vectors/negative cases | HOST PASS | Python3.13/cryptography46.0.3 `python -m unittest -v test_reference.py` ผ่าน11tests รวมtamper/wrongtopic/key/replay/duplicate/encoding/limits |
| สร้างสำเนาแอปและ TypeScript starter | BUILD PASS | helperสร้างสำเนาชื่อ/package/iconใหม่; Node24.19.0 `npm ci --ignore-scripts --no-audit --no-fund`, `npm run typecheck`, `npm run export:android` exit0 |
| README/code parity, pinout, links, quiz, images | QA PASS | 3.inoตรงcodeblocks; fixedpins; repository/quiz9บท/109ภาพ; Markdownlocaltargetsทุกไฟล์/ลิงก์READMEแล็บ+รากผ่าน; SVG/PNG8ไฟล์ตรวจด้วยสายตา |
| BLE Security2 / SoftAP Security2 บน Android+ESP | NOT RUN | secure runtime/native adapter ยังเป็นงานพัฒนาตาม phase |
| MQTT AEAD command/ACK, broker migration จริง | NOT RUN | ยังไม่มี secure runtime end-to-end |
| Android APK/S25 Ultra, backup/Keystore/revoke | NOT RUN | ต้องทำ native module และ build บนเครื่องจริง |
| 1000 commands/24h soak/OTA/eFuse | NOT RUN | อยู่นอกขอบเขตชุดเริ่มต้น ห้ามยืมผล Lab07 |

## ทำซ้ำ

```powershell
python scripts/validate-pinout.py
./scripts/validate-repository.ps1
./scripts/validate-quizzes.ps1
python -m venv build/lab09-python
build/lab09-python/Scripts/python.exe -m pip install -r labs/lab09-secure-homeguard/protocol/requirements.txt
Set-Location labs/lab09-secure-homeguard/protocol
../../../build/lab09-python/Scripts/python.exe -m unittest -v test_reference.py
```

FQBNทั้ง3ชุด: `esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi,CDCOnBoot=default,UploadSpeed=115200` ใช้Flash/PSRAMตามบอร์ดจริง คำสั่งcompileเหมือนAGENTSที่รากเปลี่ยนpathเป็นcodeของLab09

Android bundleตรวจJS/Hermes/assets ไม่ใช่APKหรือผลติดตั้งบนS25Ultra มีคำเตือนสิ่งแวดล้อมNO_COLOR/FORCE_COLORจากMetro ไม่ใช่TypeScript error. Sourceapp/lockdependenciesจากLab07คงเดิม เปลี่ยนเฉพาะmetadata/icon/Appstarterในสำเนาใหม่

ZIPตรวจCRC/bytesตรงsourceครบ5รายการ(3.ino+secrets.example.h+HOW_TO_USE) ไม่มีsecrets.h. ไฟล์`Secure-IoT-Level-5.md`เก็บต้นฉบับของผู้สอนเพื่อให้ลิงก์ในบทใช้ได้ ไม่ได้ทำเครื่องหมายP0–P9ของต้นทางเป็นผ่าน

## ข้อจำกัดที่สำคัญ

บทนี้ส่งมอบคู่มือปฏิบัติ/ชุดเริ่มต้น ไม่ใช่ผลพัฒนาsecureend-to-endครบระบบ NativeKotlinSecurity2/Keystore/MQTTและESP-IDFruntimeยังเป็นงานตามphase ข้อมูลรหัสในpreflightเป็นDEMO ไม่ส่งคีย์หรือเปิดtransport. ArduinoGCMเพียงcompileผ่าน ยังไม่มีผลKotlin↔Cบนเครื่องจริง PublicEMQXไม่มีACLแยกบัญชี จึงไม่ผ่านprivateACLgateโดยนิยาม

## ผลพัฒนาแอปและทดสอบบนเครื่องจริง

27 พฤศจิกายน 2026 — ตารางส่วนบนเป็นผลของ starter รอบเดิม ตารางนี้เป็นผลของแอป native และ development firmware ที่เพิ่มตามคำสั่งผู้สอน

| รายการ | ผลรอบนี้ | หลักฐาน / ขอบเขต |
|---|---|---|
| React Native UI / TypeScript / ESLint | PASS | Node24.19.0, Expo58.0.26, RN0.88.1; npm test3/3, typecheck/lint exit0 |
| Native Kotlin protocol tests | HOST PASS | Gradle :homeguard-security:testDebugUnitTest: 7tests, failures0/errors0/skipped0; RFC5869/Python AES-GCM vectors, strict parser, handshake/replay/expiry |
| Standalone Android APK | BUILD PASS | :app:assembleRelease exit0; arm64-v8a+x86_64; package com.esp32lab.homeguard, version2.0.0/code1, minSDK30/target36; debug signing สำหรับแล็บ |
| ติดตั้งบน Samsung S25 Ultra | HARDWARE PASS | ADB install -r: Success; SM-S938B / Android17; เปิดแอปได้โดยไม่เปิด Metro |
| Broker probe บนโทรศัพท์จริง | HARDWARE PASS | broker.emqx.io:8883 TLS+MQTT CONNECT สำเร็จ; ไม่ใช่การยืนยัน ESP |
| Crypto บนโทรศัพท์จริง | HARDWARE PASS | UI PASS: HKDF / AES-GCM / ปฏิเสธ tag ที่ผิด |
| Emulator แยกสำหรับ QA | EMULATOR PASS | ติดตั้ง/เปิดหน้า Dashboard/Broker/Security, TLS+MQTT probe และ native selfTest สำเร็จ ไม่ยืมผลเป็น ESP |
| S3 runtime build/upload | BUILD + UPLOAD PASS | CLI1.6.0/core3.3.13 warnings all; 1,489,021bytes, RAM53,536bytes; upload/hash verify สำเร็จ มีสำรอง Flash16MB เฉพาะเครื่องก่อนเขียน ไม่มี eFuse change |
| Wi-Fi บน ESP32-S3 เปล่า | HARDWARE PASS | COM7/ESP32-S3 rev0.2/Flash16MB; Serial status wifi=1 ไม่บันทึก SSID/password ในรายงาน |
| Relay GPIO ตอนเริ่มต้น | HARDWARE PASS เฉพาะระดับขา | Serial gpio16=1/gpio17=1 คือ OFF สำหรับ activeLOW; เปิด input buffer อ่านระดับขา ไม่มีโมดูล Relay หรือโหลด |
| เปิด BLE และ discovery | HARDWARE PASS เฉพาะเปิด/ค้นหา | physical BOOT เปิด HG_… แล้วโทรศัพท์ค้นพบ ไม่ใช่หลักฐานว่า Security2/key exchange จบ |
| Owner authentication บน Android | PARTIAL | เปิด native BiometricPrompt ได้; ยังไม่ยืนยันผลครบ workflow |
| BLE / SoftAP Security2 จับคู่ครบ | PENDING | ยังไม่บันทึก PASS end-to-end; รอรอบ physical window และการยืนยันบนโทรศัพท์ |
| MQTT AEAD command/ACK/GPIO, replay, reboot, lease | NOT RUN ครบระบบ | ต้องจับคู่สำเร็จก่อน ทดสอบเฉพาะอุปกรณ์ UUID ของผู้สอน ไม่มีการส่งคำสั่ง topic ของผู้อื่น |
| Broker stage/test/commit/rollback สองฝั่ง | NOT RUN | source มี workflow; ไม่อ้างผลจาก profile probe |
| DHT11 / OLED SH1106G / หน้าสัมผัส Relay / ปุ่ม GPIO18 | NOT RUN | บอร์ดรอบนี้ไม่มีโมดูล; DHT ส่ง valid=false/null, BOOT0 ใช้เฉพาะ HG_BARE_BOARD |
| private ACL, encrypted NVS, Secure Boot, signed OTA, 1000 commands, 24h soak | NOT RUN | development board / public broker; ไม่มี production release หรือ independent review |

APK ล่าสุด `app-blueprint/mobile/dist/apk/s3-homeguard-2.0.0.apk` SHA256 `CD71B9B3BAA93E931486C9435CC17F574CB8C24063CD11B498404A019CB08126` ไม่ commit APK/binary ที่ฝัง Wi-Fi ใน firmware. วิธีสร้างซ้ำอยู่ใน [คู่มือแอป](app-blueprint/mobile/README.md)

### ข้อแก้จากการทดสอบจริง

- แก้ชื่อฟิลด์ service ของ BLE ระหว่าง TypeScript/native ให้ตรงกัน
- จอง BLE memory ด้วย header ของ core เช่นเดียวกับ WiFiProv หลังพบ panic ตอนเริ่ม controller รอบแรก
- ปิด provisioning ด้วย manager deinit ที่รอ cleanup เอง หลังพบ race จาก asynchronous stop ตามด้วย deinit ทันที; ต้องทดสอบ timeout/เปิดซ้ำต่อก่อนนับ PASS ทั้งวงจร
- ตั้ง latch GPIO ผ่าน ESP-IDF ก่อนเปิด output เพื่อให้ activeLOW เริ่ม OFF และเปิด input buffer สำหรับตรวจระดับขาจริง แยกจากการยืนยันหน้าสัมผัส Relay
- เพิ่มรหัสข้อผิดพลาดแบบสั้นใน UI และ native log เฉพาะรหัส ไม่มี credential/payload/exception dump

หลักฐานสาธารณะ: [โทรศัพท์ Dashboard](app-blueprint/mobile/evidence/homeguard-s25-dashboard.png), [TLS probe](app-blueprint/mobile/evidence/homeguard-phone-brokers.png), [crypto selfTest](app-blueprint/mobile/evidence/homeguard-phone-security.png). ไม่เก็บภาพหน้ารหัส/Pair form ใน repo. Serial/Flash backup/ข้อมูลจับคู่เก็บเฉพาะ build/lab09-private ที่ ignore

คำเตือน build ที่ยังมี: deprecated Android/Gradle APIs, optional Netty Reactor BlockHound service และ Node module-type warning; ไม่มี TypeScript/lint error และไม่ปิด TLS validation เพื่อแก้ build

### ปรับการแสดงปุ่มจับคู่ — รุ่น2.0.1

ผู้สอนหา action จับคู่ไม่พบ จึงแยกปุ่มและสถานะออกจาก ScrollView เป็น footer เหนือเมนูล่าง เห็น **จับคู่กับ ESP32-S3** ตั้งแต่เปิดหน้าเพิ่มอุปกรณ์และขณะเลื่อนกรอกข้อมูล ขั้นตอนบนหน้าแอปบอก Wi-Fi/broker → BOOT2s → รหัสท้องถิ่น → จับคู่/ยืนยันบนโทรศัพท์

- npm test3/3, typecheck และlint exit0; Gradle native7/7 + assembleRelease exit0
- APK2.0.1/code2 SHA256 `6342C9717FACC40A2116CF78C7176CE5058316B9026966C1D8C7D6CBB316825E`; ADB install-r Success บน Samsung ตัวเดิม
- UI hierarchy1440×3120: ScrollViewจบY2141; ข้อความปุ่มจับคู่Y2341–2421 เห็นได้ทั้งก่อน/หลังเลื่อนฟอร์ม ไม่มีภาพหน้ารหัสในrepoเพราะFLAG_SECURE
- ผลนี้ยืนยันการแสดงaction ยังไม่แทนผลBLE/SoftAP/AEADที่ต้องผ่านการจับคู่จริงในตารางข้างบน

### ค้นสาเหตุและแก้ driver/lifecycle — รุ่น2.0.3

- [PAIRING_DIAGNOSIS](PAIRING_DIAGNOSIS.md) มีหลักฐาน GATT connected/services discovered/MTU256 เทียบต้นทาง SDK และเอกสาร Android/Espressif/Expo
- คืน readDescriptor สองจุดที่ถูกตัดพร้อม log; เพิ่ม discovery regression2tests และจำลองบั๊กเดิมเฉพาะ QA cache แล้วพบ failures2 ตามคาด จากนั้นคืน source และตรวจ native9/9ผ่าน (protocol7+discovery2)
- ปรับ chunk64bytes, แจ้ง BLE_DISCOVERY_TIMEOUT หลัง20s, รอ Android Wi-Fi dialog โดยไม่แลกกุญแจขณะ HomeGuard ไม่อยู่ foreground
- Firmware ใช้ core เป็นผู้ deinit เมื่อ END เพียงจุดเดียว, UUID storage อยู่ครบอายุ API และ Wi-Fi recovery หลัง local window ปิด; compile1490197bytes/core3.3.13/warnings all ผ่านไม่มี warning และ upload/hash verify ผ่าน
- APK2.0.3/code4 SHA256 `E20F948CE75FCB9FD7F354447A4A08B97B5EB48D2F1D5EAA63C2AF5C870757A7`; assembleRelease/install-r ผ่าน และ native crypto selfTest บน Samsung ผ่านอีกครั้ง
- Serial หลัง flash: paired0/wifi1/mqtt0/authenticated0/gpio16=1/gpio17=1 แสดง OFF และยังไม่มี session; จับคู่สองวิธี/คำสั่ง AEAD/broker migration บน firmware ล่าสุดยังไม่ผ่านการตรวจครบ ไม่ย้ายสถานะ mock ไปเป็น hardware PASS

### 27 พฤศจิกายน 2569 — จับคู่ BLE บน Samsung S25 Ultra กับ ESP32-S3 บอร์ดเปล่า

- รุ่น 2.0.4 สร้างแพ็กเกต SRP แรกไม่ได้บนทั้ง Samsung และ Android emulator ด้วยรหัสทดสอบสาธารณะ รุ่น 2.0.5 ผ่านทั้งสองเครื่องหลังคงฟิลด์ protobuf ใน R8 และตรวจอ่านแพ็กเกตกลับ
- APK 2.0.5/code 6 SHA256 `F089F93105D9A8F48A7E1ADC763CF96DE001605B5FAC533D5650FB060A3571D8` ติดตั้งผ่าน ADB ทดสอบ JVM 11 ข้อ และ TypeScript 3 ข้อผ่าน พร้อม typecheck และ lint
- Firmware core 3.3.13 ย้าย token buffer ขนาดคงที่ออกจาก stack ของ endpoint คอมไพล์ได้ 1,490,109 ไบต์ อัปโหลดบน COM7 และตรวจ hash ผ่าน
- จับคู่ด้วย BOOT และรหัสจาก USB Serial ผ่าน BLE Security2 patch 1 แอปแสดง “จับคู่สำเร็จ” และแสดงอุปกรณ์ในหน้าหลัก บอร์ดยืนยันว่าบันทึกข้อมูลและตอบ proof แล้วปิดหน้าต่างจับคู่ สถานะหลังจับคู่ `paired=1 wifi=1 mqtt=1 authenticated=0 gpio16=1 gpio17=1` ก่อนผู้ใช้เปิดเซสชันควบคุม
- กด **ยืนยันตัวตน / เปิดเซสชัน** บน Samsung บอร์ดบันทึก `Fresh application session confirmed` และ `authenticated=1` ทดสอบปุ่มเปิด–ปิดทั้งสองช่องจากหน้าหลัก อ่านระดับพินจาก ESP32-S3 ผ่าน USB Serial หลังแต่ละคำสั่ง

| คำสั่งจากโทรศัพท์ | ค่าที่อ่านกลับจากบอร์ด | รุ่นสถานะ |
|---|---|---|
| เปิดช่อง 1 | GPIO16 LOW | 2 |
| ปิดช่อง 1 | GPIO16 HIGH | 3 |
| เปิดช่อง 2 | GPIO17 LOW | 4 |
| ปิดช่อง 2 | GPIO17 HIGH | 5 |

หลังคำสั่งสุดท้าย แอปแสดง “ยืนยันแล้ว” และ OFF ทั้งสองช่อง บอร์ดรายงาน `paired=1 wifi=1 mqtt=1 authenticated=1 gpio16=1 gpio17=1` ค่าบน Serial เป็นค่าที่อ่านจาก GPIO ในโหมด active LOW บอร์ดทดสอบไม่มีโมดูลรีเลย์และ DHT11 หน้าแอปแสดงขีดสำหรับอุณหภูมิและความชื้น การจับคู่ทาง SoftAP และการย้าย broker บนฮาร์ดแวร์ยังรอตรวจ

### 27 พฤศจิกายน 2569 — ตรวจเมนูและธีม S3 HomeGuard 2.1.1

- ใช้การ์ดช่องควบคุมจาก SmartHome เป็นแนวทาง เมนูล่างอ้างอิง AROWA-V2 และปุ่มลอยตรงกลางจาก FloatingNavDemoV.2 ภาพประกอบจาก ImageGen บันทึกไว้ใน [IMAGE_SOURCES.md](../../assets/IMAGE_SOURCES.md)
- รุ่น 2.1.0 ติดตั้งบน Samsung และตรวจว่าโปรไฟล์ที่จับคู่ไว้ยังอยู่ รีเลย์วางคู่กัน การเปิดเมนูตั้งค่า การจัดการโปรไฟล์ และการทดสอบ crypto บนโทรศัพท์ผ่าน
- รุ่น 2.1.1 เพิ่มธีมขาว–น้ำเงิน แถบเมนูลอย ภาพประกอบ 6 ชิ้น และเปลี่ยนให้ผู้ใช้เริ่มยืนยันตัวตนจากการ์ดรีเลย์ ปุ่มจับคู่อยู่ท้ายฟอร์มในพื้นที่เลื่อนเดียว
- TypeScript typecheck, ESLint, TypeScript 3 tests, JVM 11 tests, Android release build และการตรวจลายเซ็น APK ผ่าน SHA-256 ของ APK สำหรับ Samsung คือ `6FFCDF6492FDD18EA739ECC0A31D43801A68BF04CDEDEB23125C2CAA8209C351`
- รุ่น 2.1.1 บน Android Emulator ตรวจครบ 5 เมนู ตำแหน่งปุ่ม + ตรงกลาง ภาพท้ายหน้าตั้งค่า การเปิดหน้าจัดการ Broker และ crypto self-test เลื่อนถึงปุ่มจับคู่ขนาดเล็กท้ายฟอร์มได้ โดยแถบเมนูล่างอยู่ที่เดิม
- เริ่มติดตั้งรุ่น 2.1.1 บน Samsung แล้ว ระหว่างรอคำตอบหน้าต่าง Google Play Protect เครื่องยังรายงานรุ่น 2.1.0 การเปิดเซสชันและสั่ง GPIO จากการ์ดบนรุ่น 2.1.1 ยังรอตรวจ

### 27 พฤศจิกายน 2569 — ตรวจหน้าจอเต็มขนาดและภาพการ์ดใน S3 HomeGuard 2.1.2

- ตรวจภาพ 2.1.1 บน Samsung พบว่าขนาดชั้นภาพคิดจากพื้นที่ด้านในที่มี padding จึงเห็นขอบว่างด้านขวาและด้านล่าง โหมด `cover` ตัดยอดกังหัน รุ่น 2.1.2 ใช้กรอบภาพเต็มพื้นที่การ์ดและคงอัตราส่วนเดิม จากภาพบนเครื่องเห็นบ้านทั้งสองหลังและกังหันครบ
- ติดตั้ง APK 2.1.2/code 9 บน Samsung S25 Ultra ผ่าน ADB โปรไฟล์ที่จับคู่ไว้แสดงในหน้าหลัก ตรวจว่า Android ซ่อน status bar และ navigation bar ขณะอยู่ในแอป เมนูห้าปุ่มมีพื้นที่กดกว้างเท่ากัน เรียงระดับเดียวกันและชิดขอบล่างจอ 1440 × 3120
- สลับครบทั้งห้าแท็บ ตรวจไฮไลต์ที่หน้าหลักและตั้งค่าจากภาพจริง เลื่อนฟอร์มเพิ่มอุปกรณ์ไปยังช่อง BLE เปิดและปิดคีย์บอร์ดแล้วกลับหน้าหลักแบบเต็มจอ การทดสอบ UI รอบนี้ไม่ได้ส่งข้อมูลจับคู่หรือสั่ง GPIO
- TypeScript typecheck, ESLint, TypeScript 3 tests, JVM 11 tests, Android release build และการตรวจลายเซ็น APK ผ่าน SHA-256 `FE8B5B8FCBA2577348C7AAB0948F15ADB5D7E5C0BE47047CD147E7E872FF1583`

### 27 พฤศจิกายน 2569 — เมนู Compact Center ใน S3 HomeGuard 2.1.3

- ใช้ตัวอย่างที่ 4 จาก `D:\AI\TestUI\FloatingNavDemoV.2\src\shared\navigation\BottomBar\variants\CompactCenterTabBar.tsx` พร้อมสัดส่วนจาก `StaticTabItem` และ `CenterActionButton`: แถบขาวสูง 78dp บวก safe area ระยะด้านข้าง 24dp ปุ่มกลาง 50dp ไม่มีข้อความใต้ปุ่ม แท็บด้านข้างใช้ไอคอนเดิม 20dp และเปลี่ยนสีไอคอน/ข้อความ ไม่มีเงา พื้นไฮไลต์ รอยเว้า หรือปุ่มเลื่อนตามแท็บ ปุ่มกลางเปลี่ยนเป็นสีน้ำเงินเข้มเมื่อเลือกหรือกดตามที่ผู้สอนขอ
- BUILD/HOST: `npm run typecheck`, `npm run lint`, `npm test` (3/3), `pwsh -File scripts/build-homeguard.ps1 -BuildRoot D:\AI\hg09\app -Architectures arm64-v8a` (JVM 11/11 + release APK), `apksigner verify --verbose`, `python scripts/validate-pinout.py` และ `git diff --check` ผ่าน exit 0
- HARDWARE UI: `adb install -r` ผ่านบน Samsung S25 Ultra ตรวจ package เป็น 2.1.3/code 10 โปรไฟล์อุปกรณ์เดิมยังแสดงในหน้าหลัก สลับครบ **หน้าหลัก/อุปกรณ์/เพิ่ม/ประวัติ/ตั้งค่า** โดยแต่ละหน้ามีแท็บ selected เพียงหนึ่งปุ่ม และ bounds ของห้าแท็บคงเดิมทุกหน้าในจอ 1440 × 3120; กลับหน้าหลักเมื่อเสร็จ
- ตรวจภาพหน้าหลักและตั้งค่าจากเครื่องจริง เห็นไอคอน/ข้อความที่เลือกเป็นสีน้ำเงินบนพื้นขาว ปุ่มกลางอยู่ตำแหน่งเดิม หน้าเพิ่มอุปกรณ์ตรวจจาก UI hierarchy เพราะ `FLAG_SECURE` ป้องกันภาพหน้าจอ เก็บหลักฐานรอบนี้ในพื้นที่ local ชั่วคราว ไม่เพิ่มภาพแล็บ
- APK SHA-256 `20778C26FC93BF460A5F53377CE76E63F7F294F5260D73260D72B18C173B7546` ไม่มีการส่งข้อมูลจับคู่หรือสั่ง GPIO รอบนี้; firmware compile/การจับคู่/คำสั่ง AEAD เป็น NOT RUN สำหรับการแก้เมนูครั้งนี้

### 27 พฤศจิกายน 2569 — ปรับชุดเอกสารและกระบวนการสร้างแอป

- ปรับ Markdown 27 ไฟล์ใน Lab09/สเปกราก/หน้า landing รวมคู่มือใหม่ BUILD_GUIDE, CONFIGURATION, USER_GUIDE, KNOWN_ISSUES; แยก reference/starter, Release SRP gate, schema/contract ที่ source ใช้จริง, สถานะปัจจุบัน และงานที่ยังไม่ผ่าน
- DOC/HOST: ตรวจ local targets/anchors 216 จุด, JSON20กรอบ, fenced blocks/Python syntax, SQL2ตารางเทียบsource, HG-01–12ครบ และ P0–P9ของผู้เรียนคงว่าง ผ่าน ตัวอย่าง encrypted Cmd/ACK/State ใน SYSTEM_WORKFLOW คงเดิม
- `python scripts/validate-pinout.py`, Python reference11/11, `npm run typecheck`, `npm run lint`, `npm test`3/3 และ `pwsh -File scripts/build-homeguard.ps1 -BuildRoot D:\AI\hg09\app -TestsOnly` (JVM11/11) ผ่าน exit0; repository validatorผ่าน9บท/123ภาพและtracked-file policy; diff whitespace ผ่าน
- NOT RUN รอบเอกสาร: Release APK/self-testบนโทรศัพท์/จับคู่/GPIO/firmware compile-flash ไม่ได้ทำซ้ำ ไม่มี app/firmware code เปลี่ยนจากงานเอกสารนี้ ผล hardware ที่อ้างในคู่มือเป็นผลเดิมที่ระบุเวอร์ชันไว้ ไม่ปิดงาน SoftAP/migration/production gate จากการแก้ Markdown

### 27 พฤศจิกายน 2569 — ภาพหน้าจอแอปจริงประกอบท้ายบท

- ตรวจแอปที่ติดตั้งบน Samsung Galaxy S25 Ultra SM-S938B เป็น version2.1.3/code10 แล้วจับ ADB screencap6หน้า: Dashboard/อุปกรณ์/ประวัติ/ตั้งค่า/MQTT Broker/ความปลอดภัย PNGต้นฉบับ1440×3120 ไม่ปรับเนื้อหาภาพ
- เพิ่มแกลเลอรีท้าย READMEแล็บและ app-blueprint/README พร้อมคำอธิบายและลิงก์ภาพเต็ม ลงทะเบียน6ภาพใน IMAGE_SOURCES และตรวจด้วยสายตาว่าไม่มี password/root/session key
- คง FLAG_SECURE ของฟอร์มจับคู่ ไม่จับภาพหรือปิดกลไกนั้นเพื่อทำเอกสาร ภาพDashboardแสดงSessionล็อก/sensorไม่มีข้อมูล ส่วนประวัติเป็นข้อมูลเดิม
- รอบนี้ตรวจ UI/ภาพ/เอกสาร ไม่ได้จับคู่ เปิดSession สั่งGPIO หรือทดสอบsensor/หน้าสัมผัสRelayเพิ่ม กลับหน้าDashboardหลังจับภาพ
- QAภาพผ่าน PNG/CRC/decompression/ขนาด1440×3120 และ SHA-256ตรงไฟล์จากโทรศัพท์ทั้ง6ภาพ; gallery links/alt/captionsและทะเบียนภาพผ่าน; repository validator9บท/129ภาพและตัวตรวจพินผ่าน ไม่มี app/firmware codeเปลี่ยนในงานภาพ

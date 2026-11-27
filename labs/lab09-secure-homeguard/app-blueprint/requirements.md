# ข้อกำหนดตรวจรับ

| ID | ต้องได้ |
|---|---|
| HG-01 | แอป S3 HomeGuard/package `com.esp32lab.homeguard`/icon ใหม่ ติดตั้งคู่ Lab07; UI พื้นขาวไม่มีเงา ใช้ source เดิมเป็นฐาน |
| HG-02 | Dashboard: Relay unknown/pending/on/off/error, DHT valid/invalid/age, แยก broker connected กับ device authenticated |
| HG-03 | จับคู่ BLE และ SoftAP ได้จริงอย่างละรอบ; manual SRP username/password จาก OLED/Serial ท้องถิ่น QR optional |
| HG-04 | กด physical เปิด pairing120s, จำกัด attempts/cooldown, ปิด transport เมื่อสำเร็จ/หมดเวลา; ห้าม MQTT เปิด pairing |
| HG-05 | Native module สร้าง root32bytes/device ด้วย CSPRNG ส่งผ่าน Security2 เก็บ wrapped key ใน Keystore/ไฟล์private exclude backup; ESP encrypted NVS ใน production |
| HG-06 | โปรไฟล์ broker: host/TLS port/WSS path/CA/credential handles; เปลี่ยนได้โดย local authenticated stage→test→confirm→commit ไม่ฝังค่ายใน business logic |
| HG-07 | TLS1.2+ ตรวจ CA+hostname+เวลา; public EMQX เป็นโหมดห้องเรียน private มีบัญชีเฉพาะ/ACL default deny; ไม่มี plaintext/insecure fallback |
| HG-08 | AES256-GCM/HKDF directional keys/envelope/session/sequence/deadline ตรง Level5; Kotlin/C/Python vectors ต้องตรงกัน |
| HG-09 | fresh session ทุก reboot/reconnect/resume, retained/replay ไม่ actuate, QoS1 dedup32, command deadline5s, ไม่ queue control ขณะ offline |
| HG-10 | Relay safeOFFก่อนnetwork, queue8, rate10/s, lease30s renew10s, local safetyไม่รอMQTT; DHT11อ่านอย่างน้อย2s |
| HG-11 | config export มีเฉพาะ UUID/endpoint/pins/ข้อกำหนดTLS ไม่มีคีย์/รหัสผ่าน; ไม่มีฟังก์ชัน flash โดยตรงใน starter |
| HG-12 | physical revoke/reset, root rotation/recovery, logs sanitized, Android background/resume/backup tests และ signed OTA/production hardening แยก gate |

เจ้าของหนึ่งโทรศัพท์ต่ออุปกรณ์หลายตัวใน V1 ไม่รองรับ remote first-pairing หรือส่งสิทธิ์เจ้าของทางแชตเป็น feature เริ่มต้น ไม่มีเว็บกลางหรือ cloud database บังคับ

## กระบวนการพัฒนาและเอกสารที่ต้องส่งมอบ

เกณฑ์ HG-01–12 ข้างบนยังเป็นเป้าหมายตรวจรับ ไม่ใช้สถานะแอปอ้างอิงแทนหลักฐานของผู้เรียน อ่านความต่างจาก source ปัจจุบันใน [CONFIGURATION](CONFIGURATION.md) และ [mqtt-contract](mqtt-contract.md)

| ID | เกณฑ์เพิ่มเติมเพื่อให้สร้างซ้ำและใช้งานได้ง่าย |
|---|---|
| DX-01 | เลือก reference/starter ชัดเจน มี environment/lockfile/local module/plugins และ Release APK ที่ทำ SRP packet self-test ก่อน hardware pairing |
| DX-02 | คู่มือตั้งค่าระบุว่าแต่ละค่าอยู่ไฟล์/หน้าจอใด แยก SRP/AP/Wi-Fi/Broker/root key และ bare-board/โมดูลจริง |
| DX-03 | USER_GUIDE ตรง UI ปัจจุบัน: แตะเพื่อยืนยันก่อนสั่ง, ปุ่มจับคู่ท้ายฟอร์ม, pending/unknown/State และวิธีกู้คืน |
| DX-04 | KNOWN_ISSUES มีอาการ/สาเหตุ/ข้อแก้/regression/สถานะจริง โดยเฉพาะ GATT, R8, provisioning lifecycle, buffer และ keyboard |
| DX-05 | test-plan และ TEST_REPORT แยก HOST/BUILD/PHONE/BOARD/MOCK/NOT RUN; source commit/status, artifact hash และข้อจำกัดตรงเวอร์ชันที่ส่ง |

UI ใช้เมนูห้าแท็บ Compact Center ตาม [design](design.md) ส่วน schema ใน [data-model](data-model.md) ต้องอ้าง implementation จริงและระบุ migration ที่ยังไม่ทำ ห้ามลด HG เรื่องสอง transport/private ACL/production hardening เพื่อให้ checklist ผ่าน

# ข้อมูลที่เก็บจริงและขอบเขต migration

> ฉบับสาธารณะ: เอกสารอธิบายแบบและผลอ้างอิงสำหรับผู้เรียนสร้างเอง source แอป/secure runtime สำเร็จเก็บแยกสำหรับผู้สอน ไม่ได้อยู่ใน repository นี้

อ่านร่วมกับ [design](design.md), [CONFIGURATION](CONFIGURATION.md) และ [SYSTEM_WORKFLOW](../SYSTEM_WORKFLOW.md)

## 1. SQLite ของแอปอ้างอิงปัจจุบัน

Source `mobile/src/homeguard/database.ts` เปิด `homeguard-public.db`, WAL และ `user_version=3` มีตารางดังนี้:

```sql
CREATE TABLE IF NOT EXISTS device_profiles(
  handle TEXT PRIMARY KEY,
  device_id TEXT NOT NULL,
  label TEXT NOT NULL,
  broker_id TEXT NOT NULL
);
CREATE TABLE IF NOT EXISTS events(
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  code TEXT NOT NULL,
  handle TEXT,
  time INTEGER NOT NULL
);
```

- device_profiles เป็น public cache ที่ refresh จาก Native device list ภายใน transaction ไม่ใช่แหล่ง root key/สิทธิ์ควบคุม
- events รับรหัส `[A-Z_]{1,64}` เก็บล่าสุดไม่เกิน200รายการ อ่านแสดงล่าสุด30รายการ; time เป็นเวลาแสดงเหตุการณ์ ไม่ใช้เป็น anti-replay clock
- **ยังไม่มี** broker_profiles/sensor history/migration Lab07 แบบสมบูรณ์ตามแบบจำลองที่เคยเสนอ การตั้ง user_version=3 ไม่ยืนยันว่ามี ALTER/migration ครบทุก schema เก่า

## 2. Native vault เป็นแหล่งข้อมูลหลักของ owner/Broker

`KeyVault.kt` เก็บ JSON ที่เข้ารหัสด้วย wrapping key จาก Android Keystore ใน app-private `noBackupFilesDir/homeguard-v1/` อ้างด้วย opaque handle ต่อ record Broker/device มี secrets ตามหน้าที่ ส่วน JS ได้เฉพาะ public projection

Root/session keys ไม่เก็บใน SQLite/AsyncStorage/export/log; session key/seq/deadline/pending อยู่ใน runtime และถูกทิ้งเมื่อปิด Session Native ต้องเห็น raw key ชั่วคราวระหว่างคำนวณ จึงไม่อ้างว่า root อยู่ใน secure hardware ตลอดเวลา ส่วน hardware security-level/key-use authentication policy ยังต้องตรวจรับเพิ่ม

Broker profile กับ device record ผูกด้วย broker_id; candidate migration เก็บ pending broker/tx แยกจาก active ไม่แก้ active endpoint ฝ่ายโทรศัพท์อย่างเดียว

## 3. Firmware

`HgStore.h` เก็บ config ใน Preferences/NVS รวม identity, root/kid, Wi-Fi, active/candidate Broker และ bootstrap budget รุ่น development ยังไม่เป็น encrypted NVS; physical revoke ล้าง config เจ้าของแต่คง device UUID ตาม source ปัจจุบัน

เป้าหมาย rotation ใน Level5 มี policy ใหม่ของ kid/root/revocation แต่ runtime นี้ kid=1 และยังไม่มี automation หมุน root ครบ จึงไม่กล่าวว่า physical reset เปลี่ยน UUID เสมอ

## 4. ถ้าจะย้ายข้อมูล Lab07

1. สำรอง public label/history ที่ผู้ใช้อนุญาตก่อนเปลี่ยน schema/package
2. ออกแบบ migration จริงและ tests จาก schema เก่า ไม่สร้างแต่เลข user_version ใหม่
3. profile เก่าต้อง requires_pairing/unknown/offline; ห้ามแปลง `home/<student_id>` เป็นเจ้าของ secure UUID อัตโนมัติ
4. session/pending/retained data เก่าไม่เป็นหลักฐาน online และไม่ส่งคำสั่งค้างใหม่
5. ห้ามย้ายกุญแจด้วย plaintext backup; หากเปลี่ยน package/vault ให้ใช้ authenticated recovery/re-pair ตาม policy

ขั้นตอนนี้เป็นงานที่ต้อง implement/test ก่อนอ้าง migration PASS ไม่ใช่สิ่งที่ helper คัดลอก source Lab07 ทำเสร็จแล้ว

## 5. Public export และการลบ

public bootstrap มี UUID/endpoint/พิน/ข้อกำหนด TLS ตาม exporter ไม่ใส่ root/SRP/Wi-Fi/Broker password หรือ bearer credential handle การลบในโทรศัพท์ลบ local record ส่วนการเพิกถอนบอร์ดต้อง physical revoke และ Private Broker credentials ต้องเพิกถอนแยกตาม [USER_GUIDE](USER_GUIDE.md)

# การออกแบบแอปและ development runtime Lab09

> ฉบับสาธารณะ: เอกสารอธิบายแบบและผลอ้างอิงสำหรับผู้เรียนสร้างเอง source แอป/secure runtime สำเร็จเก็บแยกสำหรับผู้สอน ไม่ได้อยู่ใน repository นี้

อ่าน [requirements](requirements.md), [ค่าตั้ง](CONFIGURATION.md) และ [ปัญหาที่พบ](KNOWN_ISSUES.md) ก่อนเปลี่ยน integration

## 1. Source ที่ใช้อยู่

```text
app-blueprint/mobile/
  App.tsx                         routes / BottomBar
  src/homeguard/                  UI, public cache, native bridge
  modules/homeguard-security/     Expo module, Kotlin, vendor SDK/proto, tests
  plugins/                        manifest, CMake, native configuration
  scripts/build-homeguard.ps1     สร้าง native ใน cache พาธสั้น
code/lab09_s3_homeguard/           Arduino3.3.13 + ESP-IDF APIs / SH1106G
protocol/                        Python vectors และ negative tests
```

พาธในภาพนี้อ้างอิงชุด Lab09 ใน repository ไม่ใช่คำสั่งให้สร้าง firmware/ อีกชุดที่ไม่มี implementation หากสร้างโปรเจกต์ใหม่ให้บันทึกพาธจริงของทั้งสองฝั่งใน PLAN ก่อน

## 2. แบ่งหน้าที่และข้อมูล

- JS แสดง UI และถือ opaque device handles/public metadata; เรียก Native ผ่าน bridge ไม่รับ root/session key
- Native ถือ vault, Security2, TLS MQTT, AEAD/session, monotonic timer และ sanitized events
- ESP MQTT callback รับ bounded packet เข้าคิว ส่วน protocol ตรวจ tag/session/seq/schema/deadline ก่อนส่ง actuation; Safety task เขียน GPIO และตรวจ deadline/Lease ซ้ำ
- Native source/consumer rules อยู่ใน local module; สิ่งที่ Prebuild สร้างต้องย้อนกลับมาได้จาก plugins ไม่แก้เฉพาะ mobile/android/ หรือ cache
- SQLite เป็น public cache/events ตาม [data-model](data-model.md) ไม่ hydrate session/pending เก่ามาเป็นสิทธิ์ควบคุม

## 3. UI และลำดับที่ผู้ใช้เห็น

เปิดแอปโหลดรายการก่อนแล้วแสดงเซสชันล็อก แตะการ์ดครั้งแรกเรียก Android authentication/เปิด Session เมื่อได้ authenticated State จึงแตะครั้งถัดไปเพื่อสั่ง `set_relay` ขณะ pending แสดงรอผล และใช้ State จากบอร์ดแสดง ON/OFF

เมนูห้าปุ่มคือ หน้าหลัก/อุปกรณ์/เพิ่ม/ประวัติ/ตั้งค่า แบบ Compact Center: แถบขาว 78dp บวก safe area, ปุ่มกลาง 50dp ไม่มีคำใต้ปุ่ม, ไอคอนด้านข้าง 20dp/ชื่อ10.5sp เปลี่ยนสี ไม่มีเงาหรือวงกลมไฮไลต์ตามแท็บ การตั้งค่า Broker/อุปกรณ์/ความปลอดภัยเข้าจากตั้งค่า

หน้าเพิ่มเป็นฟอร์ม ScrollView เดียว ปุ่มจับคู่อยู่ท้ายฟอร์ม ต้องเลื่อนถึงได้ทั้งขณะมี/ไม่มีคีย์บอร์ด ไม่ใช้ตำแหน่ง footer ของรุ่นเก่าเป็นข้อรับรองรุ่นปัจจุบัน ภาพบ้านใช้สัดส่วนเดิมและตรวจขอบจริงบนโทรศัพท์

ทุกหน้าต้องแยก broker connected, device authenticated, Relay unknown/pending/on/off/error และ sensor valid/fresh การจับคู่ไม่สำเร็จต้องบอกขั้นที่หยุดพร้อม retry/cancel

## 4. Lifecycle และ async SDK

- background ล็อก MQTT control ทันทีและปิด Session; resume ไม่ใช้ Session เก่า
- ข้อยกเว้น SoftAP system dialog คงเฉพาะ pending network request รอ foreground ก่อน SRP/ส่ง key ไม่คงสิทธิ์ remote control
- มี operation/generation token ป้องกัน callback ของ connection เก่ากลับมาเปลี่ยน state ใหม่
- BLE discovery ต้องอ่าน descriptors จริง ไม่ซ่อน side effect ใน log; UUID/buffer ที่ SDK เก็บ pointer ต้องมี lifetime เพียงพอ
- provisioning ของ Arduino core ขอ stop แล้วรอ END ให้ deinit จุดเดียว จากนั้นล้าง SRP/transfer และกลับ Wi-Fi/MQTT
- config แบ่ง chunk64 ตาม baseline MTU256; จัดการ error/timeout/cancel ก่อนเริ่มรอบถัดไป

## 5. เกณฑ์ออกแบบก่อนใช้ hardware

P0 ต้องได้ Release APK ที่สร้าง/อ่าน SRP packet ได้บน Android พร้อม JVM regression เพื่อกัน R8 ที่เคยทำให้ build ผ่านแต่ใช้จริงไม่ได้ P3 จึงตรวจ byte contract/crypto และ P4/P5 ตรวจ Session/transport บนบอร์ดแยกกัน

ก่อนเปิด GPIO ต้องผ่าน TLS/AEAD/Session/safety gates ไม่มี mock success หรือ legacy plaintext fallback เมื่อ Native ไม่พร้อม UI ต้องล็อกและแจ้งเหตุผล

## 6. ย้าย Broker และ production

candidate credential แยกจาก active; root key ไม่เปลี่ยน ส่ง candidate ผ่าน local Security2 เปิด Session ที่ใหม่แล้วให้ผู้ใช้ confirm ภายใน timeout บันทึก commit สองฝั่งและมี recovery สำหรับ ACK หาย/ไฟดับ/reboot รายงานปัจจุบันยังไม่ยืนยัน atomicity ครบทุก failure ต้องใช้ [test-plan](test-plan.md)

Private ACL, root rotation/revoke, hardware-backed key policy, encrypted NVS/Flash, Secure Boot/Signed OTA และ independent review เป็นเกณฑ์ที่ยังต้องทำเพิ่ม LAN discovery/QR เป็นส่วนเสริม ไม่แทน physical confirmation หรือ authentication

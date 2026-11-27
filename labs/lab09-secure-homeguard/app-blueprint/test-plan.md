# ใบตรวจรับและหลักฐาน Lab09

เริ่มผลของโปรเจกต์ใหม่เป็น **NOT RUN** ทุกข้อ ผลอ้างอิงเดิมอ่าน [TEST_REPORT](../TEST_REPORT.md) และสถานะที่ยังเปิดใน [KNOWN_ISSUES](KNOWN_ISSUES.md) การทดสอบต้องตรง source/APK/firmware ที่ส่งมอบ

## 1. ตรวจตามลำดับก่อน hardware pairing

| Gate | วิธีตรวจ / Expected | ประเภทหลักฐาน |
|---|---|---|
| G0 Source/environment | version/lockfile/plugin/local module ครบ บันทึก git commit+status; ไม่มีข้อแก้เฉพาะ cache | HOST |
| G1 JS/public boundaries | npm typecheck/lint/test ผ่าน; Native ไม่พร้อมต้องล็อก UI | HOST |
| G2 Native integration | JVM protocol/discovery/SRP tests ผ่าน; regression ต้องจับบั๊กเดิมได้ | HOST/JVM |
| G3 **Release APK** | assembleRelease/signature/install ผ่านตาม architecture; บน Android self-test ผ่าน HKDF/GCM/rejectTag/**SRP packet** | BUILD + PHONE |
| G4 Broker probe | TLS/MQTT positive; wrong CA/hostname/time fail closed; Public ACL=N/A | PHONE/BOARD แยก |
| G5 Firmware safety | core/board/partition ตรง, GPIO OFF ก่อน network, read GPIO/flags; ไม่อ้าง contact เมื่อบอร์ดเปล่า | BUILD + BOARD |
| G6 Pair | BLE และ SoftAP success แยกกัน; UUID/proof/paired=1, timeout/cancel/denied/wrong password/เปิดรอบใหม่ | PHONE + BOARD |
| G7 Session/command | Hello/Challenge/Confirm/ACK/State, ทั้งสองช่อง ON/OFF และอ่าน GPIO; pending/unknown ถูกต้อง | PHONE + BOARD |
| G8 UI/lifecycle | ห้าแท็บ/Compact Center, ปุ่มจับคู่ท้ายฟอร์ม, keyboard/scroll, background/resume/no stale control | PHONE |

ไม่ข้าม G3 เพราะเคยพบ R8 ทำให้ SRP ใช้ไม่ได้ทั้งที่ build/JVM ผ่าน การตรวจ GATT mock ใน G2 ไม่แทน G6 และ Broker probe G4 ไม่แทน G7

## 2. Matrix เชิง protocol และ production

| Gate / อ้างอิงต้นทาง | ต้องลอง / Expected |
|---|---|
| Crypto T01–03/T11/T20 | Kotlin/C/Python vectors ตรง; wrong tag/topic/key/nonce, duplicate fields/oversize/uint64 reject; ไม่มีการสั่ง GPIO |
| Session T04–08 | exact replay/retained/old session ไม่ actuate; new seq+cached request ไม่สั่งซ้ำ; Hello ไม่เปลี่ยน active ก่อน Confirm; deadline ตรวจซ้ำก่อน GPIO |
| TLS/ACL T09–10 | CA/hostname/time ผิด fail closed; Private cross-device publish/subscribe deny; Public profile ไม่ให้ PASS ACL |
| Pair T14 | ทั้งสอง transport/manual credentials/physical120s, attempts/cooldown ที่บอร์ดและแอปแยก; config/proof encrypted; ปิด transport เมื่อสำเร็จ/หมดเวลา |
| Broker migration | stage/test/confirm/commit ถึงทั้งสองฝั่ง; CA/บัญชีผิด/unavailable/reboot/ไฟดับ/ACK หาย/timeout มี recovery ไม่ค้างครึ่งทาง |
| Runtime T12–13/T15/T19 | safe boot/glitch/brownout/Lease/network loss/background, flood/queue/rate ตาม HG-10, 1000 commands/24h soak พร้อม p50/p95/p99 |
| Secrets T18/T21–22 | root/SRP/password ไม่อยู่ใน log/SQLite/export; vault/backup/revoke/old phone/rotation; APK/source ไม่มี secrets จริง |
| OTA T16–17 | signed/unsigned/corrupt/old/power-cut/rollback/self-test; hardening บอร์ดสำรองและ gate ก่อน eFuse |

ส่ง malformed/flood/replay tests เฉพาะ offline หรือ Broker ส่วนตัวที่มีสิทธิ์ ไม่โจมตี Public Broker และใช้โหลด DC จำลองตามขอบเขตแล็บ

## 3. UI และผลที่ต้องอ่านให้ถูก

- [ ] เปิดแอปเห็นรายการที่บันทึก แต่ control session ล็อก
- [ ] แตะครั้งแรกเปิด authentication/Session; ไม่ส่ง ON ทันที
- [ ] ปุ่มควบคุมพร้อมเมื่อ authenticated และมี State ของช่องนั้น
- [ ] หลังสั่งแสดง pending; ใช้ application ACK/request ID และ State ไม่ใช้ PUBACK
- [ ] timeout เป็น unknown แล้วขอ State ไม่ส่ง ON ซ้ำ/ไม่แสดง success ล่วงหน้า
- [ ] ไม่มีโมดูล DHT แสดง invalid/null; State/Serial GPIO ไม่ใช่ contact feedback
- [ ] ปุ่มจับคู่ท้ายฟอร์มเห็นได้เมื่อเลื่อนและปิดคีย์บอร์ด เมนูล่างไม่บัง
- [ ] background ล็อก; กลับมาไม่ hydrate Session/pending เก่า
- [ ] SoftAP system dialog รอได้ แต่ไม่แลก key จน foreground; deny/cancel ฟื้นตัวได้
- [ ] Profile/device list ยังอยู่หลัง upgrade ที่ package/signature เดิมเข้ากัน

## 4. แบบบันทึกต่อกรณีทดสอบ

```text
Case ID / HG / issue ที่เกี่ยวข้อง:
วันที่ / ผู้ตรวจ:
Source commit / working-tree changes:
APK versionCode / hash / architecture / signature:
Firmware/core/hash / board / flags / โมดูลที่ต่อจริง:
Transport / Broker mode (ไม่ใส่รหัส):
Preconditions / ขั้นตอน:
Expected:
Actual:
Result: PASS / FAIL / NOT RUN / N/A
Evidence type: HOST / BUILD / PHONE / BOARD / MOCK
หลักฐานที่ล้าง secret แล้ว:
ข้อจำกัด / NOT RUN เพราะ:
```

PASS ต้องมี actual+evidence, FAIL คงไว้จนแก้/ตรวจซ้ำ, NOT RUN ระบุเหตุผล ส่วน N/A เช่น ACL ของ public ต้องไม่ใช้แทน production PASS เมื่อแก้ source หลังทดสอบให้ระบุผลใดต้องรันใหม่

## 5. ความต่างที่ต้องตรวจรับเพิ่ม

Telemetry QoS1 เทียบเป้าหมาย0, ESP keepalive30 เทียบ60, filter20 packets/s เทียบcommand10/s, client cooldown เทียบserver control และ SQLite schemaจริงเทียบmigrationที่เสนอ ให้บันทึก DECISIONS/tests ก่อนอ้าง HG ผ่าน ห้ามแก้เป้าหมายให้ดูตรงโดยไม่มีเหตุผล

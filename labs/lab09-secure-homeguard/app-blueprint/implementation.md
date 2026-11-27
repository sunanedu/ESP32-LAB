# แผนพัฒนา P0–P9 — checklist ของผู้เรียน

เริ่มทุกข้อเป็น `[ ]` แม้แอปอ้างอิงมีผลผ่านบางส่วนแล้ว ใช้ [BUILD_GUIDE](BUILD_GUIDE.md) เมื่อรัน reference และ [KNOWN_ISSUES](KNOWN_ISSUES.md) เป็นรายการ regression ก่อนทำงานแต่ละช่วง

## วิธีใช้แผน

ทำตามลำดับ เก็บ PLAN/PROGRESS/DECISIONS/TEST_REPORT ของโปรเจกต์ตนเอง ทุก phase บันทึก source/version/คำสั่ง/exit code/evidence/NOT RUN แยก HOST, BUILD, PHONE และ BOARD ก่อนทำเครื่องหมายผ่าน เกณฑ์ HG อยู่ใน [requirements](requirements.md)

| Phase | งาน / สิ่งส่งมอบ | Gate ก่อนทำถัดไป |
|---|---|---|
| P0 | เลือก reference หรือ starter, กำหนดพาธ/identity/lockfile, local module/plugins/consumer rules และ release self-test | Android/ESP skeleton build; JVM discovery/protocol ตามที่มี; **Release SRP packet self-test บน Android** และ plugin source ครบ (I-01/02/11) |
| P1 | safe boot/Relay/Lease/queue, SH1106G/DHT, bare-board flags และ Serial diagnostics | compile + host safety cases; ตรวจ GPIO บนบอร์ดตามอุปกรณ์ที่มี ไม่อ้างโมดูลที่ไม่ได้ต่อ |
| P2 | profiles/TLS/CA/hostname/time และ Private ACL plan | probe positive/negative; Public ACL=N/A; ยังไม่ให้ network เปิด Relay |
| P3 | Kotlin/C/Python vectors, strict parser/AEAD/HKDF/AAD/nonce | vectors ตรงและ negative cases ผ่าน ข้อมูลลับไม่ผ่าน JS/SQLite/log |
| P4 | Hello/Challenge/Confirm, fresh Session, seq/deadline/cache, ACK/State | stale/replay/retained/duplicate/reboot/reconnect tests; ยังไม่ให้ UI แสดง success จาก PUBACK |
| P5 | BLE และ SoftAP Security2, local physical window/manual credentials, proof, lifecycle/cancel | ทดสอบทั้งสองวิธีแยกจริง รวม denied/timeout/ปิด–เปิดรอบใหม่; ตรวจ I-03–08 |
| P6 | UI/SQLite public cache, five tabs/Compact Center, keyboard, public export และ broker migration | UI/Native integration จริง; test stage/confirm/commit/rollback/recovery สองฝั่ง ไม่ใช้ migration mock เป็น PASS |
| P7 | permission/background/resume/network switch/replay/Lease/queue/rate/1000 commands/24h soak | บันทึก metrics และ deviation โดยไม่ลด requirement ให้ตรง source |
| P8 | revoke/rotation/recovery, signed OTA/rollback, hardening plan บอร์ดสำรอง | negative/failure cases; ไม่ burn eFuse จากแผนนี้ ไม่เผย signing key |
| P9 | ตรวจทุก HG/test-plan, release/source/lockfiles/คู่มือ/ผลจริง | independent review และ manual acceptance; คง NOT RUN/FAIL ที่เหลือ |

P0 self-test ตรวจ packaging/SRP packet ได้ก่อนครบ crypto/session implementation ของ P3/P4 โดยใช้ test vectors สาธารณะ ไม่ใช่การเปิดสิทธิ์ควบคุมก่อนเวลา

## Checklist

- [ ] P0 Environment/identity/source และ release packaging gate (HG-01/08)
- [ ] P1 Hardware preflight/local safety/Lease (HG-02/10)
- [ ] P2 TLS/Broker/Private ACL (HG-06/07)
- [ ] P3 Byte contract/crypto/strict parser (HG-05/08)
- [ ] P4 Session/replay/deadline/ACK/State (HG-09)
- [ ] P5 BLE **และ** SoftAP/physical window/proof/lifecycle (HG-03/04/05)
- [ ] P6 UI/public storage/export/broker migration (HG-01/02/06/11)
- [ ] P7 Runtime/Android/fault/load/soak (HG-10/12)
- [ ] P8 Revoke/rotation/recovery/OTA/hardening (HG-12)
- [ ] P9 Independent review/ส่งมอบและระบุ NOT RUN ตามจริง

หากติดขัดให้ปิดงานย่อยที่ทำได้โดยไม่อาศัย hardware ก่อน เช่น R8 packet generation หรือ GATT mock แล้วคง gate ที่ขาดหลักฐานไว้ ไม่ให้ build หรือ emulator แทนผลอุปกรณ์จริง

# คำสั่งให้ AI พัฒนาและแก้ Lab09 ทีละ gate

ใช้กับ [implementation](implementation.md) และ [KNOWN_ISSUES](KNOWN_ISSUES.md) แทนการสั่ง “ทำแอปให้เสร็จทั้งหมด” ครั้งเดียว ใส่ path จริงของ source/เอกสารใน prompt ก่อนส่ง ห้ามแนบรหัสจริงหรือ Flash backup

## 1. เริ่มงาน: สำรวจและเลือกเส้นทาง

> อ่าน AGENTS.md ที่ใช้กับ source, requirements.md, design.md, implementation.md, BUILD_GUIDE.md, CONFIGURATION.md และ KNOWN_ISSUES.md ของ Lab09 ใน repository ต้นทาง อ่าน Level5 และ DECISIONS เพื่อทราบเกณฑ์เดิมกับข้อปรับ ตรวจ git status, source/lockfile, Native module/plugins, board/core/partition และผล TEST_REPORT ล่าสุด ระบุว่าจะรัน reference หรือพัฒนา starter แยกจากกัน ห้ามแก้ Lab07 ทำ PLAN/PROGRESS/TEST_REPORT ของโปรเจกต์นี้พร้อม HG mapping และ NOT RUN ก่อนทำเฉพาะ P0 อย่าขอรหัสผ่านผ่านแชต

## 2. P0: Build และ Release packaging ก่อน pairing

> ทำ P0: ตรวจ Node24/JDK17/Android SDK, local Expo module, config plugins และ consumer rules ให้ source สร้าง native ซ้ำได้ ใช้ BuildRoot สั้นตาม script ห้ามเก็บข้อแก้เฉพาะ generated android/ หรือ cache รัน typecheck/lint/JS tests/JVM tests และสร้าง Release APK ที่เปิดได้โดยไม่พึ่ง Metro ตรวจ SRP first-packet create/parse และ crypto self-test บน Android release เพื่อจับ R8 regression I-02 ตั้งแต่ต้น หากไม่มี Native ให้ล็อก control ห้าม fake success รายงานคำสั่ง/exit code/version/hash และ hardware NOT RUN ตามจริง

## 3. P1–P2: Firmware safety และ TLS

> ทำ P1 ตามพิน S3 เดิม: Relay16/17, DHT4, SH1106G8/9, Button18; bare-board BOOT0 เป็น flag แยก ตั้ง OFF ก่อน network ใช้ Safety task/Lease/queue และตรวจ deadline อีกครั้งก่อน GPIO บันทึกอุปกรณ์ที่มีจริง ถ้าไม่มี sensor ให้ invalid/null ตรวจ source/core/partition/Flash backup ก่อนเสนอขั้น upload dev board ไม่ burn eFuse จากงานนี้

> ทำ P2: โปรไฟล์ Broker Public lab/Private แยก CA/hostname/เวลาและบัญชี phone/device ตรวจ probe positive/negative ไม่มี insecure fallback แยก MQTT connected จาก device authenticated Public ACL=N/A ยังไม่เปิด Relay ด้วย plaintext ระบุค่าจริงที่ต่าง requirement ใน DECISIONS

## 4. P3–P4: Protocol ก่อนปลดล็อก UI

> อ่าน mqtt-contract.md, data-model.md และ SYSTEM_WORKFLOW.md ทำ P3: HKDF/AES-GCM directional keys, AAD exact Topic, strict JSON/length/duplicate/canonical seq/Base64 ตรวจ Kotlin/C/Python bytes และ negative cases ใช้ public test vectors เท่านั้น root/session key อยู่ Native และไม่ log plaintext/secret

> ทำ P4: Hello/Challenge/Confirm/ACK, fresh Session ทุก reconnect/reboot/resume, seq ต่อ directional key, command deadline5s ตรวจซ้ำก่อน GPIO, dedup cache32 แยก replay envelope เดิมที่ถูกทิ้งกับ new seq+requestเดิมที่ตอบ cache อย่าใช้ MQTT PUBACK เป็นผล GPIO ผูก request ID และ State; timeoutแสดงunknownแล้วขอState ไม่มีoffline ON replay ตรวจ negative casesก่อนเปิดcontrol

## 5. P5: Pairing และ async lifecycle

> ทำ P5 ทีละชั้นตาม I-01–08: Release SRP packet PASS ก่อน; GATT descriptors/read callbacks ต้องคง side effect เมื่อเอา log ออก UUID/buffer ต้องมี lifetime; firmware ขอ stop แล้วรอ END ให้ core deinit ครั้งเดียว ใช้ Security2 patch1 ทั้ง BLE/SoftAP ห้าม fallback กรอกรหัสจาก local physical120s และตรวจ UUID ก่อนส่ง root/config ใช้ bounded chunk64/proof/timeout/cancel ตรวจ ESP memory/stack ไม่สรุปจับคู่ PASS จาก GATT connected แยกผลสอง transport พร้อมการเปิดรอบใหม่

> ตรวจ SoftAP system Wi-Fi dialog โดยเฉพาะ: control ล็อกทันทีเมื่อ pause เก็บได้เฉพาะ pending network request ระหว่าง dialog เริ่ม SRP/ส่งข้อมูลเมื่อ HomeGuard กลับ foreground ตรวจ accept/deny/cancel/background และ cleanup ถ้าไม่มี hardware ให้คง NOT RUN ไม่ยืม BLE PASS

## 6. P6: UI/ข้อมูล/การย้าย Broker

> ทำ P6: UI ห้าแท็บแบบ Compact Center ปุ่มกลาง50dp/ไม่มีเงา แท็บเปลี่ยนสี การ์ดรีเลย์แตะครั้งแรกเพื่อยืนยันแล้วรอ State ครั้งถัดไปจึงสั่ง ตรวจจอจริง/keyboard/scrollถึงปุ่มจับคู่ท้ายฟอร์ม SQLiteเก็บpubliccache/eventsตามschemaจริง ไม่อ้างmigrationLab07ที่ไม่ได้ทำ secretsอยู่Native ไม่มีlegacytransportfallback

> พัฒนา broker migration stage/test/confirm/commit พร้อม transactionID ทั้งสองฝั่ง ตรวจ TLS/sessionที่ใหม่ก่อนยืนยัน มีtimeout/reboot/ACKหาย/ไฟดับ/recovery ห้ามเรียกว่า atomicครบระบบจากการบันทึกฝั่งเดียวหรือprofileprobe rootไม่เปลี่ยนเมื่อย้ายBroker ส่งออกpublicconfigไม่มีsecret

## 7. P7–P9 และส่งมอบ

> ทำ P7–P9 ทีละ phase ตาม test-plan.md เก็บ evidence HOST/BUILD/PHONE/BOARD/MOCK แยกกัน ตรวจ actual rate20packets/sเทียบrequirement10commands/sและservercooldownที่ยังขาด โดยไม่ลดเกณฑ์เงียบ ๆ ตรวจ Lease/replay/reboot/revoke/rotation/privateACL/backup/load/soak/OTA ตามขอบเขต ห้ามใช้ Public Broker เป็นเป้าทดสอบโจมตี ไม่ burn eFuse/เปิดbilling/ใช้โหลด AC จาก prompt นี้ สรุปPASS/FAIL/NOTRUNและข้อจำกัดproduction พร้อมsource/lockfiles/APKhashและคู่มือ

## 8. เมื่อพบบั๊กระหว่างทาง

> หยุดการเปลี่ยนหลายชั้นพร้อมกัน ระบุขั้นสุดท้ายที่ผ่านและ source/APK/firmware versions เทียบ KNOWN_ISSUES แยก fact/hypothesis ทำ reproduction ที่เล็กที่สุด เพิ่ม regression ที่จับบั๊กเดิม เก็บข้อแก้ใน source ต้นทาง แล้วรันเฉพาะ gate ที่กระทบ อัปเดต contract/config/user guide/DECISIONS/TEST_REPORT ที่เกี่ยวข้อง ไม่ขอให้ผู้ใช้จับคู่ซ้ำเพื่อรอแก้ build ที่ทำบนคอมพิวเตอร์ได้ก่อน

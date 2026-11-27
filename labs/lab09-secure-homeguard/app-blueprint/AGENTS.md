# AGENTS — S3 HomeGuard

อ่าน README → requirements → design → implementation ของชุดนี้ก่อนพัฒนา และอ่าน [ต้นทาง Level5](../../../Secure-IoT-Level-5.md) กับ [DECISIONS](../DECISIONS.md) ใช้ BUILD_GUIDE/CONFIGURATION/KNOWN_ISSUES ตาม phase และอ่าน mqtt-contract/data-model/test-plan ก่อนงาน security/storage ห้ามใช้คู่มือรุ่นเก่าแทน source โดยไม่รายงานความต่าง

1. ต่อยอด source Lab07 ในสำเนาใหม่ ไม่แก้ Lab07 และไม่เปิด plaintext MQTT control เดิมใน Lab09
2. UI React Native + TypeScript; Native Kotlin ทำ crypto/Keystore/provisioning/MQTT ห้ามให้ JS เห็น root/session key
3. ESP32-S3/SH1106G ใช้พินเดิม: OLED8/9, Relay16/17, DHT4, Button18
4. ขั้นจับคู่ต้องสำเร็จทั้ง BLE และ SoftAP ด้วย Security2; มี manual input ไม่บังคับกล้อง ห้าม fallback Security0
5. broker profile แยก public-lab/private; TLS ตรวจ CA+hostname และ application AEAD ทุกคำสั่ง; แค่ connected ไม่ใช่ authenticated
6. อย่าฝัง key/password ใน source, logs, SQLite, AsyncStorage, header ที่ export หรือ APK ใช้ test keys เฉพาะ vectors
7. แต่ละ phase ต้อง build/test ผ่านก่อนทำเครื่องหมาย [x]; ใช้ negative tests ไม่อ้าง hardware PASS จาก unit/mock
8. ถ้ายังไม่มี native module ให้ disable การควบคุมพร้อมเหตุผล ห้าม return fake success หรือใช้ legacy transport แทน
9. เก็บ Android native module ในโฟลเดอร์ที่ติดตาม source ได้ เช่น local Expo module `mobile/modules/homeguard-security/`; อย่าเก็บงานเฉพาะใต้ android/ ที่ gitignore หรือถูก prebuild เขียนทับ
10. ไม่สร้างบัญชี/billing ไม่ flash โหลด ไม่ burn eFuse อัตโนมัติ; production gate แยกจาก dev board
11. ถ้า API/เวอร์ชันไม่แน่ใจตรวจ official docs; lock version หลัง build ทั้ง Android และ ESP ผ่าน
12. ส่งรายงานสั้นภาษาไทย: ทำอะไรแล้ว คำสั่ง/exit code อะไรผ่าน อะไร NOT RUN และเหตุใด

13. P0 ต้องตรวจ Release SRP packet/self-test บน Android เพิ่มจาก JVM tests เพื่อกัน R8 regression; ทดสอบที่ไม่ต้องใช้บอร์ดให้จบก่อนขอ physical pairing
14. เมื่อตัด log ต้องคง side effect เช่น readDescriptor; ตรวจ async lifetime, single deinit ของ core, SoftAP foreground และ chunk bounds ตาม KNOWN_ISSUES
15. ใช้ data-model ของ source จริง ไม่อ้าง migration/schema หรือ atomic broker transaction ที่ยังไม่ทดสอบ บันทึกค่าจริงเทียบ requirement โดยไม่ลดเกณฑ์เงียบ ๆ
16. เมื่อเปลี่ยนพฤติกรรมให้ปรับคู่มือเจ้าของข้อมูลและ TEST_REPORT พร้อมกัน เก็บ checklist ผู้เรียนว่างจนมีผลของโปรเจกต์นั้น

หาก AGENTS ถูก helper คัดลอกไปโปรเจกต์ใหม่ ให้หาเอกสารจากบริบทต้นทางที่ helper ระบุท้ายไฟล์ ไม่สมมติว่าคู่มือทุกไฟล์ถูกคัดลอกไปด้วย และไม่รันคำสั่งของ reference บน starter ที่ยังไม่มี native module/scripts

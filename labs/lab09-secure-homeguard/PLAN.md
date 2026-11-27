# แผนสร้าง Lab09 — S3 HomeGuard

อ้างอิง [Secure-IoT-Level-5.md](../../Secure-IoT-Level-5.md) และข้อปรับของผู้สอน: ต่อยอด React Native จาก Lab07, ใช้ ESP32-S3/SH1106G, broker เปลี่ยนได้, จับคู่ใกล้ตัวอย่างน้อย BLE และ SoftAP โดยไม่บังคับกล้อง

## ขอบเขตชุดเริ่มต้นของบทเรียน

- [x] L9.0 สร้างบทเรียนไทยครบ template พร้อมใบงาน/คำถาม/ภาพ/ลิงก์จากสารบัญ
- [x] L9.1 Firmware Arduino core3.x: ตรวจวงจรและปุ่มเปิดหน้าต่างจับคู่, crypto self-test, TLS broker probe แยกชุด ไม่เปิดคำสั่ง Relay จากเครือข่ายก่อนระบบตรวจสิทธิ์ครบ
- [x] L9.2 ชุด Markdown สำหรับ OpenCode พัฒนาทั้ง React Native native security และ ESP-IDF; mapping ทุกข้อจาก Level5 และบันทึกส่วนที่ยังไม่ implement
- [x] L9.3 เครื่องมือสร้างสำเนาแอป Lab07 เป็น S3 HomeGuard เปลี่ยน package/name/icon เปิดหน้าเริ่มต้นที่ล็อกการควบคุมเดิม มีตัวเลือก BLE/SoftAP และส่งออก config ที่ไม่มี secret
- [x] L9.4 ตรวจ compile, native app starter typecheck, protocol vectors, negative tests, links, images, quiz, secrets policy และ commit

## งานพัฒนา secure end-to-end ของผู้เรียน

P0–P9 อยู่ใน [app-blueprint/implementation.md](app-blueprint/implementation.md) ต้องทำและทดสอบทั้งสองฝั่งก่อนใช้กับ Relay จริง การมีเอกสาร/ภาพ/UI หรือตรวจ build ผ่านไม่ทำให้ gate เหล่านี้ผ่านโดยอัตโนมัติ

## แอปอ้างอิงและแผนทำงานต่อ

มี Native/firmware จริงและผล BLE+Session+GPIO บนบอร์ดเปล่าแล้วตาม [PROGRESS](PROGRESS.md) การรัน source ใช้ [BUILD_GUIDE](app-blueprint/BUILD_GUIDE.md) ต่างจากการใช้ helper starter เพื่อสร้างใหม่ คู่มือ [CONFIGURATION](app-blueprint/CONFIGURATION.md), [USER_GUIDE](app-blueprint/USER_GUIDE.md), [KNOWN_ISSUES](app-blueprint/KNOWN_ISSUES.md) และ [SYSTEM_WORKFLOW](SYSTEM_WORKFLOW.md) เป็นบริบทสำหรับรอบถัดไป

- [x] จัดสารบัญตามงานและแยก reference/starter พร้อม build/config/user/known issues
- [x] ปรับ P0 ให้ Release SRP packet/self-test เป็น gate ป้องกัน R8 ก่อนขอ hardware pairing
- [x] ปรับ contract/schema/status ให้แยก source ปัจจุบัน เป้าหมาย และผลทดสอบจริง
- [ ] ตรวจ SoftAP end-to-end/lifecycle ครบทั้ง success/negative รอบใหม่
- [ ] ตรวจ broker migration/recovery สองฝั่งและ fault cases
- [ ] ตรวจ runtime/โมดูลจริง/privateACL/production gates ตาม test-plan

งานค้างด้านบนไม่ถูกทำเครื่องหมายผ่านจากการแก้ Markdown การพัฒนาแอปใหม่ใช้ P0–P9 เดิมเป็นลำดับ พร้อม gate ที่ขยายจากบทเรียนข้อผิดพลาด

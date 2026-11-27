# AGENTS — Source แอปอ้างอิง S3 HomeGuard

ใช้กติกา [AGENTS ของ blueprint](../AGENTS.md) ร่วมกับ [คู่มือ source](README.md), [BUILD_GUIDE](../BUILD_GUIDE.md), [CONFIGURATION](../CONFIGURATION.md) และ [THIRD_PARTY](THIRD_PARTY.md) ก่อนแก้ integration อ่าน Level5/DECISIONS ตามพาธของ blueprint

- Source หลักอยู่ที่นี่; Native ใน `modules/homeguard-security/` และ config ใน `plugins/` ห้ามแก้เฉพาะ generated `mobile/android/` หรือ build cache
- ตรวจ git status และรักษางานเดิม รัน npm typecheck/lint/test และ native/Release tests ที่เกี่ยวข้องเมื่อเปลี่ยน code; งานเอกสารตรวจ contract/examples/links โดยไม่อ้างผล hardware ใหม่
- เปลี่ยน UI ต้องตรวจห้าแท็บ/การ์ดล็อก/ปุ่มจับคู่ท้ายฟอร์ม/keyboard/scrollบนรุ่นที่ build จริง และปรับ USER_GUIDE
- ห้าม fallback ไป MQTT.js plaintext, ส่ง root/session key ให้ JS, หรือตัด side effect พร้อม log
- คง release consumer rules และ SRP first-packet self-test ตรวจ lifecycle/Security2 ทั้งสอง transport แยกกันตาม test-plan
- ระบุ PHONE/BOARD/HOST/BUILD/NOT RUN ตามหลักฐานจริง ไม่ใช้เวอร์ชันเลขเดียวหรือภาพ UI เป็นหลักฐาน secure end-to-end

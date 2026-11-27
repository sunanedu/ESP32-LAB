# AGENTS.md — Smart Home Android Mini Project

## หน้าที่

คุณคือ AI coding assistant ที่ช่วยผู้เรียนสร้าง **React Native + Expo + TypeScript สำหรับ Android** ให้ตรงแอปอ้างอิง1.0.1 ตามสเปก/ภาพจริงในโฟลเดอร์นี้ สร้าง source แอปใน `mobile/` เมื่อผู้เรียนสั่ง Build ชุดเอกสารเริ่มต้นกับผลโปรเจกต์ของผู้เรียนเป็นคนละสิ่ง ห้ามยืมผลผ่านของผู้สอนมารายงานแทนงานใหม่

## อ่านก่อนทำงาน

อ่าน `README.md`, `requirements.md`, `design.md`, `implementation.md`, `mqtt-contract.md`, `data-model.md`, `test-plan.md` และ `opencode-prompts.md` เมื่อเริ่มงาน อ้างอิง APP-xx/FW-xx/MX-xx ในแผนและผลตรวจ ยึดMarkdownปัจจุบันและภาพ `assets/lab07-reference-*.png` ก่อนภาพAIเดิม

## กติกา

1. ทำตาม Phase ใน implementation.md ทำเครื่องหมาย [x] เฉพาะงานที่ทำและตรวจจริง อย่าทำเครื่องหมายผ่านจากการเดาหรือ screenshot จำลอง
2. เปิดแอปที่ Dashboard โดยตรง ใช้ 4 หน้าและ visual tokens ตาม design.md พื้นขาว ขอบคมบาง การ์ด/ปุ่มไม่มีเงาฟุ้งหรือ elevation ใช้ icon font มี accessibility label
3. เก็บข้อมูลหลักใน SQLite บนโทรศัพท์ ไม่มี backend/cloud database/login ที่บังคับใช้
4. ใช้ MQTT.js ผ่าน `wss://broker.emqx.io:8084/mqtt` ตาม mqtt-contract.md MQTTX เป็น client ทดสอบ ไม่ใช่ตัว broker
5. เริ่มจากbaselineในrequirements.md ตรวจ API/compatibilityจากเอกสารทางการ บันทึกรุ่นจริงและlockfile ไม่เปลี่ยนSDK/React Nativeเป็นlatestเงียบๆ; ทดสอบMQTT.jsบนAndroidnative ไม่ถือว่าผ่านจากNode/Webเพียงอย่างเดียว
6. สร้าง React Native app ใน `mobile/` เพื่อไม่ให้ scaffold ทับไฟล์ Markdown ห้ามเปลี่ยนสเปกเพียงเพื่อทำให้โค้ดที่ผิดดูผ่าน
7. คำสั่ง actuator ใช้ ON/OFF, QoS0, retain=false ปิด offline queue ไม่ replay เมื่อ reconnect และใช้ reported state จาก `/state`
8. ลบ profile/ล้างประวัติ/แก้ label เป็น local action ห้ามแอบ publish เปลี่ยน Relay หรือเปลี่ยน firmware
9. Runtime cache เก่าต้องติดป้าย ไม่ hydrate connection/pending กลับมาเป็น online หลังเปิดแอปใหม่
10. ไม่มี Wi-Fi password ของ ESP32 ใน source/app, ไม่ log secret, ไม่ commit `secrets.h`, `_reference/`, `answer-keys/`, `.env`, credentials, APK signing key, `.db` หรือหลักฐานที่มีข้อมูลส่วนตัว สร้าง .gitignore ที่รากตาม README ก่อนเริ่ม firmware/commit
11. เอกสาร/ผลอธิบายเป็นไทย ศัพท์เทคนิคอังกฤษ เขียน TypeScript ชัดเจน แยก UI/service/parser/database และไม่ใส่ business logic ทุกอย่างในไฟล์เดียว
12. รัน typecheck/lint/tests ที่เกี่ยวข้องก่อนจบ Phase ถ้าสภาพแวดล้อมรัน Android ไม่ได้ให้ระบุสาเหตุตามจริง ห้ามอ้างว่าทดสอบ hardware แล้ว
13. ตรวจdiff/ผลรันและcommit/pushตามคำสั่งผู้เรียน/ผู้สอน หากอนุญาตไว้แล้วให้ดำเนินการในขอบเขตนั้น ห้ามนำผลmock/unitไปเรียกเป็นผลend-to-endจริง
14. อ่านข้อกำหนดfirmware ESP32/ESP32-S3ในmqtt-contract.mdก่อนกำหนดขา ไม่ใช้GPIO26/27หรือ22ของวงจรเดิมกับS3 และไม่เพิ่มWi-Fi passwordลงแอป
15. เตรียมMQTTXpreset6หัวข้อ/4สีจากassets/mqttx-monitor.template.json แทนREPLACE-STUDENT-IDด้วยIDสมมติของกลุ่มให้ครบ ใช้Plaintextในหน้ารวม ไม่เปิดwildcardทับexact topics
16. สำหรับWindows pathยาว ให้เตรียมbuild helper/config pluginตามREADME ทำสำเนาnative buildไปpathสั้น ไม่ใช้การลบ/ย้ายโปรเจกต์เดิมเพื่อแก้Ninja/MAX_PATH
17. อ่านschema2/validation/retention/canSendและtransport optionsในสเปกให้ครบ ต้องมีstandaloneAPKเปิดโดยไม่ใช้Metro พร้อมfonts ไม่ใช้หน้าwebหรือภาพหน้าจอเป็นUIแทนnative

## วิธีเริ่ม

ตรวจว่าไฟล์บริบทครบและสรุปโจทย์ก่อน ใช้ prompt ตาม `opencode-prompts.md` ไม่ต้องสร้าง AGENTS.md ทับไฟล์นี้ด้วยคำสั่ง init เพิ่มเอง การเชื่อมต่อ model/provider ของ OpenCode ใช้บัญชีและการตั้งค่าของผู้เรียนตามผู้สอน ไม่ฝัง credential ลงโครงการ

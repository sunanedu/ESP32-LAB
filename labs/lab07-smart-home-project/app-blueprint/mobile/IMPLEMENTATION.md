# Implementation — แอปอ้างอิงที่สร้างจริง

สร้างตามคำสั่งเพิ่มเติมของผู้สอนด้วย Codex โดยใช้สเปก APP-01–12 ชุดเดียวกับที่ใช้สอน OpenCode แผน `../implementation.md` ยังเป็น checklist สำหรับโปรเจกต์ใหม่ของผู้เรียน ส่วนไฟล์นี้ติดตามผลของแอปตัวอย่าง

- [x] Phase0: ตรวจ Node/SDK/JDK และตัวอย่าง environment จากโปรเจกต์ที่ผู้สอนให้
- [x] Phase1: Expo58/ReactNative0.88.1, UI4หน้า, icon fonts, empty/loading/disabled/error และ Back/Cancel
- [x] Phase2: SQLite schema2/migration, CRUD/max5/uniqueID, cache/history, retention, transaction และ error propagation
- [x] Phase3: MQTT.js WSS, exact topics, queueQoSZero=false, client/session cleanup, retained/live และ lifecycle
- [x] Phase4: reported state, pending8s, publish failure/timeout/interrupted, สองช่องแยก outcome
- [x] Phase5a: Typecheck/lint, 19 automated tests, Android export, Debug และ standalone Release APK
- [x] Phase5b: Android emulator smoke7กรณี + WSS brokerจริง/ตัวจำลอง, ภาพ4หน้า, force-stop/persistence และ offline controls
- [x] Phase5c: ตรวจ Dashboard360dp/font1.3x, source/image policy, เอกสารและหลักฐาน
- [x] Phase5d1: โทรศัพท์ Samsung S25 Ultra Android17, standalone APK1.0.1, upgrade/CRUD/keyboard/cancel/settings/history/persistence และ UIขอบคมไม่มีเงาฟุ้ง
- [x] Phase5d2: ตรวจ ESP32-S3/สำรองFlash/compile6builds/verified upload4sketches; MQTTจริงระหว่าง Samsungกับบอร์ดเปล่า, reportedสองช่อง, invalid sensor, timeout/LWT/reconnect/no-replay
- [ ] Phase5d3: ต่อ DHT11/OLED/Relay/โหลด LED DC แล้วตรวจค่าจริง จอ แรงดัน หน้าสัมผัสและช่วงเปิดเครื่อง (รอบนี้มีเฉพาะบอร์ด)
- [ ] ทดสอบบนโทรศัพท์ Android จริงหลายรุ่น / การกดด้วย screen reader

รายละเอียดรอบ Samsung/บอร์ดจริงอยู่ใน [PHYSICAL_TEST_RESULTS.md](PHYSICAL_TEST_RESULTS.md) ส่วน emulator/ตัวจำลองอยู่ใน [TEST_RESULTS.md](TEST_RESULTS.md) การผ่าน MQTT/reported GPIO บนบอร์ดเปล่าไม่ทำเครื่องหมาย sensor/วงจรโหลดเป็นผ่าน

# ผลตรวจแอป ESP32 Smart Home

วันที่ **26 พฤศจิกายน 2026 (Asia/Bangkok)** ผลด้านล่างมาจาก source ที่รันและ APK standalone จริง ภาพ mockup AI ใช้เป็นเป้าหมายด้านหน้าตาเท่านั้น

**ผลล่าสุดรุ่น1.0.1:** ปรับเส้นขอบ UI, ติดตั้งบน Samsung S25 Ultra Android17 และทดสอบกับ ESP32-S3 จริง ดู [PHYSICAL_TEST_RESULTS.md](PHYSICAL_TEST_RESULTS.md) พร้อมภาพ/ผลดิบ รอบนี้มีเฉพาะบอร์ด จึงไม่อ้างผล DHT11/OLED/Relay ทางกายภาพ ส่วนผลในไฟล์ด้านล่างเป็นรอบ1.0.0 บน emulator/ตัวจำลอง เก็บแยกเพื่อระบุขอบเขตของแต่ละรอบ

Source ของ APK: commit [68e84bb](https://github.com/sunanedu/ESP32-LAB/tree/68e84bbefbb380b9f0789021b09f3ddf9e730f44), tag [lab07-app-v1.0.0](https://github.com/sunanedu/ESP32-LAB/raw/refs/heads/main/downloads/esp32-smart-home-preview-1.0.1.apk) ตรวจ digest ของไฟล์ที่อัปโหลดบน GitHub แล้วตรงกับ SHA256 ด้านล่าง

## Environment

| รายการ | รุ่น/ค่า |
|---|---|
| OS | Windows |
| Node / npm | 24.19.0 / 11.17.0 |
| Expo / React / React Native | 58.0.26 / 19.3.0 / 0.88.1 |
| TypeScript / MQTT.js | 6.0.3 / 5.16.1 |
| expo-sqlite / expo-crypto | 58.0.18 / 58.0.8 |
| Icon font / safe-area | @expo/vector-icons15.1.1 / safe-area-context5.11.1 |
| Test runner | Vitest5.1.2, SQLite ของ Node |
| Android native build | JDK17, Gradle9.4.1, CMake3.31.6/Ninja1.12.1, compileSdk37, targetSdk36 |
| Runtime | Pixel6 Android16/API36 x86_64 emulator, 1080×2400/density420 |
| เพิ่มเติมด้าน layout | Dashboard360dp ที่720×1600/density320, font scale1.3 |
| Broker | EMQX public WSS broker.emqx.io:8084/mqtt |
| Package | com.esp32lab.smarthome |

## คำสั่งและผลจริง

| การตรวจ | ผล |
|---|---|
| npm run typecheck | ผ่าน |
| npm run lint | ผ่าน |
| npm test | ผ่าน 19 ข้อ / 3 test files |
| npm run export:android | สร้าง Android Hermes bundle ผ่าน |
| Gradle assembleDebug x86_64 | ผ่าน ติดตั้งและรันบน emulator |
| Gradle assembleRelease arm64-v8a,x86_64 | ผ่าน รวม JS/fonts เปิดได้โดยไม่ใช้ Metro |
| Android release smoke | ผ่าน7กรณีตามตารางด้านล่าง |
| ภาพ4หน้า | จับจาก APK release จริงใน evidence/ |

APK Preview ขนาด **43,164,392 byte**; SHA256:

```text
A29E3F10F23A70C1B18B9F68D4CB1CA42FC056A6CB91F4C53161D880C7402FAD
```

ใช้ signing configuration แบบ debug/test key ที่ Expo template สร้าง ห้ามตีความว่าเป็น store-signed production release

## Automated tests: T03–T14 ที่เกี่ยวข้อง

- Parser: JSONดี/invalid-null/ศูนย์, wrong types, JSONผิด, payloadใหญ่, topicอื่น, ตัวพิมพ์ stateผิด
- Form/domain: ID injection, labelผิด, การ trim Thai/quote, แยก cache/retained/live/stale และความสดของ sensor
- SQLiteจริง: บันทึก Thai/quote, duplicate/max5, IDอ่านอย่างเดียว, migration1→2คงข้อมูล, pendingเก่าเป็น interrupted
- ล้างประวัติคง profiles/settings/snapshots, ลบ profile ลบข้อมูลอ้างถึง, cap10,000/อายุ7วัน และ errorจาก DB
- Command controller: ไม่เปลี่ยน reported ล่วงหน้า, ไม่ match retained/wrong state, timeout8s, publish failure, outcomeแยกช่อง
- Reconnect/lifecycle: offlineกดไม่ได้ ไม่มี replay, clientเก่าไม่เปลี่ยน state, invalid sensorไม่เท่ากับบอร์ด offline
- Local edit/clear/delete ไม่ publish actuator command

ผลนี้เป็น tests กับ Node SQLite/fake transport ไม่ใช่ผล hardware และไม่ใช่การทดสอบ Androidทุกยี่ห้อ

## Android release runtime

[ผลดิบ android-smoke.json](../evidence/android-smoke.json) และ `สคริปต์ทำซ้ำ` (องค์ประกอบที่ผู้เรียนพัฒนาเอง; source อ้างอิงเก็บสำหรับผู้สอน)

| กรณี | ผลจริง |
|---|---|
| เปิดแอป/เพิ่ม profile / รับ live sensor | Dashboardเริ่มโดยตรง, บันทึก SQLite และรับ28.0°C/61%RHผ่าน WSS |
| App→broker→simulator→reported state | กด Relay1ON, simulatorได้รับset, แอปรับstateแล้วแสดงเปิด |
| Edit | บันทึกชื่อใหม่แล้วกลับ Dashboardตรงชื่อ |
| Settings validation | Port0ถูกปฏิเสธ และแก้8084บันทึก/reconnectได้ |
| Force-stop / เปิดใหม่ | profileที่แก้และ relay cache ยังอยู่ใน SQLite |
| History | เปิดประวัติหลัง process restartได้ |
| Offline | ตัด clientแล้ว controls disabled ข้อมูล cache ยังอ่านได้ |

ระหว่าง force-stop/reconnect ตัวจำลองได้รับคำสั่ง ON หนึ่งครั้งจากการกดทดสอบ ไม่มีคำสั่งเดิมถูก replay หลังเปิดใหม่

ตรวจ native เพิ่มหลัง smoke: ปุ่มปิดทั้งหมดส่ง OFF แยกสองช่องและแอปได้ reported OFF ทั้งคู่; เมื่อเปลี่ยนตัวจำลองเป็น invalid ทั้งสอง sensor cards เป็นขีด/อ่านค่าไม่ได้ ขณะที่ ESP32 presence ยัง online ดู [ผลเพิ่มเติม](../evidence/extra-runtime-checks.json)

ตรวจด้วยสายตาจากภาพ nativeทั้ง4หน้า และ Dashboardที่360dp/font1.3x: ไม่มีการล้นแนวนอน ช่องข้อมูลยอมตัดบรรทัดและหน้าจอเลื่อนได้ หน้า Add/Edit มีฟอร์มภาษาไทย ข้อมูลชื่อ/ห้องใน smokeใช้ ASCII เพราะส่งจาก adb input; Thai/quote persistence ตรวจด้วย SQLite tests

## สิ่งที่ยังไม่ยืนยันในรอบ1.0.0

- รอบนี้ไม่พบ serial COM จึงไม่ได้ upload/อ่าน DHT11/OLED/สับ Relay ของ ESP32 จริง กรณี H01–H12 ของ test-plan ที่ต้องใช้บอร์ด/โหลดจริงยัง **ไม่ทดสอบ**
- ใช้ MQTT protocol simulator แทน ESP32 และไม่ได้เปิด MQTTX GUI; ทิศทางคำสั่ง/รายงานผ่าน brokerจริงถูกตรวจ แต่ไม่อ้างเป็นผลวงจรหรือ feedbackหน้าสัมผัส
- โทรศัพท์จริง, Androidหลายรุ่น, TalkBack, การใช้งานระยะยาว และ stress retention ของ commands/events ยังต้องตรวจเพิ่ม
- npm audit รายงาน3 moderateจากสาย dependency Expo fingerprint/minimatch ที่ใช้ใน tooling ไม่มี high/criticalในรอบตรวจ ไม่ใช้ audit fix --force ที่เสนอ downgrade Expo52
- Gradle/Kotlin มี deprecation warnings จาก dependencies; build Debug/Release ผ่าน ใช้เวอร์ชันที่บันทึกใน lockfile และอ่าน build helper ก่อนอัปเกรด

## Traceability APP-01–12

| APP | หลักฐาน |
|---|---|
| 01/02/03 | package/lockfile,4หน้าจอจริง,initial Dashboard,360dp/font1.3x |
| 04/07/08/09 | transport/controller tests, public WSS smoke,reported/pending/timeout/no-replay |
| 05/06 | SQLite tests + AndroidAdd/Edit/force-stop/history |
| 10 | settings validation/TLS default/no credentials fields; SQL/secret ignore policy |
| 11 | ชุด AGENTS/Markdown/prompts มีครบ; แอปอ้างอิงนี้สร้างด้วย Codexตามสเปกเดียวกัน ไม่อ้างว่าได้รัน OpenCode |
| 12 | test/build/runtime/evidence มีผลจริง ส่วนhardware/โทรศัพท์จริงระบุยังไม่ทดสอบ |

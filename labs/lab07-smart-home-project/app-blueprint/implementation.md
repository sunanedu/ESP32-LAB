# Implementation — แผนให้ผู้เรียนทำกับ OpenCode

ไฟล์นี้เป็นแผนเริ่มโปรเจกต์ใหม่ของผู้เรียน ทุกช่องเริ่ม **[ ]**; [ผลของแอปอ้างอิง1.0.1](https://github.com/sunanedu/ESP32-LAB/blob/main/labs/lab07-smart-home-project/app-blueprint/mobile/PHYSICAL_TEST_RESULTS.md) เป็นผลของผู้สอน ไม่ทำเครื่องหมายแทนงานใหม่ จดคำสั่ง/รุ่น/ผลจริงในTEST_RESULTS.mdของตนและตรวจdiffก่อนPhaseถัดไป ภาพจริงในassetsใช้เป็นเป้าหมายUI ไม่ใช่หลักฐานว่าของผู้เรียนรันแล้ว

## Phase 0 — อ่านโจทย์และเตรียมเครื่อง (APP-01/11)

- [ ] อ่าน AGENTS และไฟล์สเปกทั้งหมด สรุป 4 หน้า, local storage, topic และสิ่งที่ห้าม replay
- [ ] ตรวจ Node LTS, package manager, OpenCode/provider และ Android/Expo environment ที่ผู้สอนเลือก
- [ ] ตรึงbaseline1.0.1ตามrequirements.md ตรวจเอกสารAPI/compatibility บันทึกเหตุผลหากเปลี่ยนรุ่นก่อนinstall ไม่ใช้latestของทุกpackageเงียบๆ
- [ ] กำหนดPROJECT_NAME/packageของตน,STUDENT_IDสมมติและBOARD_TARGET ตรวจว่าIDตรงfirmware/app/MQTTXโดยไม่ใส่Wi-Fi passwordในMD
- [ ] เลือกโหมดทดลองบอร์ดเปล่าหรือต่อโมดูลครบ อ่านFW-01–10/MX-01–06และระบุhardware casesที่ยังทำไม่ได้
- [ ] ทำ traceability APP-xx → งาน/การทดสอบ และแบ่งสิ่งที่ทดสอบได้ในเครื่องกับ hardware
- [ ] เตรียม .gitignore ที่รากตาม README สำหรับ secrets.h, _reference/, answer-keys/, .env, credentials, *.db, signing keys และ generated artifacts ก่อนสร้าง firmware/commit

**จุดตรวจ:** อธิบาย MQTTX vs broker และโฟลเดอร์ mobile/ ที่จะสร้างได้ ใช้ prompt P0

## Phase 1 — Scaffold และ UI 4 หน้า (APP-01/02/03)

- [ ] Scaffold Expo TypeScript ใน mobile/ ไม่ทับ Markdown ราก
- [ ] ทำ theme tokensตามภาพจริง4หน้า ขอบ1dp/radius16/elevation0 ปุ่มกดเปลี่ยนพื้น icon fontพร้อมuseFonts/safe area/keyboard resize/scroll
- [ ] ทำ Dashboard เป็น initial route และ empty/loading state
- [ ] ทำ Add, Edit, Settings และ navigation/back/Cancel
- [ ] ทำ reusable status/sensor/relay/form components
- [ ] ใช้ fixtures เฉพาะระหว่างพัฒนา UI และระบุว่าเป็น mock; ยังไม่อ้างว่าเชื่อมจริง
- [ ] Typecheck/lint และตรวจ layout 360 dp/ฟอนต์ขยาย/ปุ่ม≥48 dp
- [ ] เตรียมbuild helperสำเนาpathสั้นและCMake3.31.6 pluginในapp.json สำหรับWindows ตามREADME; ไม่ลบsource/nativeแก้ปัญหาสุ่ม

**จุดตรวจ:** มีภาพ 4 หน้าที่เทียบ design.md ได้และปุ่มกดนำทางจริง ใช้ P1

## Phase 2 — SQLite และ CRUD (APP-05/06)

- [ ] สร้าง schema/migration/repositories และ transaction ตาม data-model.md
- [ ] Add/Edit/duplicate validation/max5 devices และ labels/icons
- [ ] Delete profile / clear history พร้อม confirm และไม่ publish
- [ ] Persist settings/install ID/cache/history และเปลี่ยน pending เก่าเป็น interrupted เมื่อเปิดใหม่
- [ ] ทำ history bottom sheet, retention และ storage-error UI
- [ ] ทดสอบ force-stop/reopen, migration, DB error, SQL parameters และ retention

**จุดตรวจ:** ไม่มีเน็ตก็เพิ่ม profile/ดู local history ได้ และ restart ไม่ทำให้ connection กลายเป็น online ปลอม ใช้ P2

## Phase 3 — MQTT service และ parser (APP-04/07/09/10)

- [ ] ทดลอง import/runtime ของ MQTT.js ใน Expo/React Native ด้วย WSS จริง
- [ ] สร้าง client เดียว, ID ไม่ซ้ำ, connect/error/disconnect events และ subscription 4 topics ต่อ device
- [ ] ตั้ง protocolVersion4, QoS0, clean session, queueQoSZero=false และ resubscribe แบบไม่ซ้ำ
- [ ] ใช้forceNativeWebSocket=true/manualConnect=true, reconnect2s/connectTimeout12s/keepalive15s; generation/sessionguardsปฏิเสธcallbackจากclientเก่า
- [ ] ทำ exact-topic/schema validation และ freshness/retained/live policy
- [ ] Settings บันทึก+reconnect ได้โดย client เก่าถูก cleanup
- [ ] Lifecycle foreground/background กลับมารอข้อมูลสด ไม่ replay
- [ ] ทดสอบ invalid payload, retained online เก่า, offline และ listener cleanup

**จุดตรวจ:** broker connected ไม่เท่ากับ ESP32 online; ไม่ต้องเปิด MQTTX เพื่อให้แอปทำงาน ใช้ P3

## Phase 4 — Monitor และคำสั่ง Relay (APP-07/08)

- [ ] แสดง sensor valid/invalid/stale และ timestamps จาก phone receive time
- [ ] reported state มาจาก state topic เท่านั้น
- [ ] Power ON/OFF, local pending, publish error, matched report, timeout 8 s และ interrupted
- [ ] Disable controls เมื่อ offline/stale/pending; ไม่ queue คำสั่ง
- [ ] ปิดทั้งหมดส่งสอง OFF แยก outcome ไม่อ้าง atomic
- [ ] บันทึกประวัติที่เกี่ยวข้องลง SQLite
- [ ] Unit/integration tests ของ state machine และ timeout ด้วย fake timers

**จุดตรวจ:** กดส่งแล้ว UI ไม่แสดงว่าสำเร็จล่วงหน้า ใช้ P4

## Phase 5a — เตรียม firmware และ MQTTX (APP-04/11/12)

- [ ] ตรวจบอร์ด/Flash/PSRAM/พอร์ตจริงก่อนcompile เลือกmappingESP32หรือS3และcore3.xตามFW-01–10
- [ ] Compile/uploadfirmwareตามบทเรียน ใส่Wi-Fi2.4GHzในsecrets.hที่ignore ตรวจSerial115200และnamespaceสมมติตรงกัน
- [ ] ทำสำเนาassets/mqttx-monitor.template.json แทนREPLACE-STUDENT-ID6แห่ง ตรวจไม่มีwildcard/secret/retainedcommandแล้วimportDesktop1.13.3
- [ ] ตรวจalias6รายการ/4สี/Plaintext,คลิกพื้นที่QoS0เพื่อfilter,onlineล่าสุด/uptimeและstateจริง ปิดMQTTXแล้วapp/บอร์ดยังทำงาน
- [ ] กรณีบอร์ดเปล่าบันทึกnoOLED/nullsensorตามจริง ไม่สร้างmockไปทับtopicของบอร์ด; ต่อโมดูลก่อนให้ผลวงจรเป็นผ่าน

**จุดตรวจ:** เห็นtrafficจริงจากอุปกรณ์ที่มีและแยก/setกับ/stateได้ ใช้P-FW/P-MX; หากมีfirmwareคอร์สอยู่แล้วให้ตรวจcontractก่อนแก้ ไม่สร้างใหม่โดยไม่จำเป็น

## Phase 5b — APK และตรวจส่งงาน (APP-12)

- [ ] ทำทุกกรณีใน test-plan.md แยก mock/unit ออกจาก end-to-end
- [ ] ทดสอบ App→ESP32→state และ MQTTX→ESP32→App
- [ ] ทดสอบ network loss, LWT, reconnect, retained state, sensor invalid และ lifecycle
- [ ] ปิดแอป/เปิดใหม่ profile/history อยู่ และไม่มี actuator command ถูกส่งเอง
- [ ] บันทึก typecheck/lint/test/build/runtime commands และผลจริง
- [ ] Build standaloneRelease arm64-v8a/x86_64รวมJS/Hermes/fonts ปิดMetroก่อนทดสอบlauncher ไม่ใช้Expo Go/webpreviewแทนผลAPK
- [ ] ทดสอบinstall-r/upgradeที่package/keyตรงกันแล้วคงข้อมูล หากsignatureผิดให้รายงานไม่uninstallเอง และไม่commitkey/APK/DBลงsource
- [ ] จับภาพ 4 หน้าและวิดีโอคำสั่ง/ผลรายงาน ลบข้อมูลส่วนตัวจากหลักฐาน
- [ ] เขียน TEST_RESULTS.md, README วิธีรันจริง และข้อจำกัดที่ยังมี
- [ ] ผู้เรียนตรวจ diff, secret และทำ commit/push ของโครงงานตนตามผู้สอน

**Definition of Done:** APP-01–12/FW-01–10/MX-01–06มีtraceabilityและหลักฐานตามขอบเขตที่ทำจริง ส่วนfull-circuitที่ไม่มีอุปกรณ์ยังไม่ผ่าน การส่งงานแบบบอร์ดเปล่าต้องตกลงขอบเขตกับผู้สอน ไม่ทำH01–H12เป็นผ่านทั้งหมดจากผลprotocol ผู้เรียนอธิบายข้อมูลlocal/retained/live/timeoutและข้อจำกัดได้ ใช้P5/P-APK/P6

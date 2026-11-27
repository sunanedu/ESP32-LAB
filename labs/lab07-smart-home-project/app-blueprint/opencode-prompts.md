# Prompts สำหรับพา OpenCode สร้างแอป

คัดลอกทีละ Phase จากโฟลเดอร์ที่มีสเปกครบ ผู้เรียน/ผู้สอนเป็นผู้ตรวจแผน diff และผลจริงก่อนขั้นถัดไป เลือก model/provider ตามการสอน ค่าใช้บริการขึ้นกับบัญชีที่ใช้ ไม่ถือว่า public broker ฟรีแปลว่า AI ทุก provider ฟรี

เป้าหมายปัจจุบันคือแอปอ้างอิง1.0.1กับMQTTX6topics/4สี ใช้ภาพจริง `assets/lab07-reference-*.png` และbaselineversions ไม่ยืมผลผ่านของผู้สอน ให้แทนค่าด้านล่างก่อนP0 (ตัวอย่างไม่ใช่IDที่ต้องใช้ร่วมกันทุกคน):

```text
PROJECT_NAME=MySmartHome
ANDROID_PACKAGE=com.example.lab07a7f2
STUDENT_ID=lab07-a7f2c9
BOARD_TARGET=esp32s3
HARDWARE_SCOPE=bare-board
```

Wi-Fi SSID/passwordใส่secrets.hในเครื่องบอร์ดเอง ไม่วางในprompt/MDสาธารณะ เปลี่ยนHARDWARE_SCOPEเป็นfull-circuitเมื่อมีOLED/DHT11/Relay/LED DCให้ตรวจจริง

OpenCode รองรับ AGENTS.md เป็น project guidance ตาม [เอกสารทางการ](https://opencode.ai/docs/instructions/) ให้เปิดโฟลเดอร์โปรเจกต์นี้ใน OpenCode Desktop หรือเปิด terminal ที่โฟลเดอร์นี้แล้วใช้ `opencode` ตามรุ่นที่ติดตั้ง ไม่ต้องสั่ง init ทับ AGENTS เดิม

## P0 — อ่านและวางแผน

```text
อ่าน AGENTS.md, README.md, requirements.md, design.md, implementation.md,
mqtt-contract.md, data-model.md และ test-plan.md ให้ครบ
สรุปโจทย์ React Native Android 4 หน้า และทำตาราง APP-xx/FW-xx/MX-xx → งาน → test ID
อ่านbaseline1.0.1และภาพจริง ตรวจเอกสารทางการของAPIรุ่นนั้นก่อนเปลี่ยนversions
บอก dependencies ที่ต้องใช้และเหตุผล วาง scaffold ใน mobile/ เพื่อไม่ทับสเปก
รอบนี้ให้แผนและตรวจเครื่องก่อน ยังไม่เขียนแอปหรือทำเครื่องหมายงานเสร็จ
แยกสิ่งที่ทดสอบในเครื่องได้กับสิ่งที่ต้องใช้ Android/ESP32
ตรวจPROJECT_NAME/ANDROID_PACKAGE/STUDENT_ID/BOARD_TARGET/HARDWARE_SCOPEที่กำหนด
ห้ามตั้งIDผู้สอนเป็นค่าdefaultในงานใหม่ ห้ามใส่Wi-Fi passwordลงapp
วางแผนสร้าง.gitignoreที่รากตามREADMEเพื่อกันsecrets.h/_reference/answer-keysและartifactsก่อนcommit
```

## P1 — สร้าง UI ตามภาพและสเปก

```text
ทำ Phase 1 ตาม implementation.md ด้วย React Native + Expo + TypeScript
สร้างใน mobile/ ทำ Dashboard เป็นหน้าเริ่มต้น Add Device, Edit Device, Settings
ใช้design.mdและภาพจริง assets/lab07-reference-dashboard.png,
assets/lab07-reference-add.png,assets/lab07-reference-edit.png,assets/lab07-reference-settings.png
ภาพAIเดิมไม่ใช่เกณฑ์ขอบการ์ดใหม่ ใช้themeค่าปัจจุบันครบfont/spacing/pressed
พื้นขาว การ์ดขอบคมทึบ 1dp มุม16dp ไม่มีเงาฟุ้งหรือ elevation รอบการ์ด/ปุ่ม ไอคอนฟอนต์ ขนาดกด≥48dp
ทำ navigation/Back/Cancel/empty/loading จริง ใช้ fixtures ที่ติดป้าย mock ชั่วคราว
ให้create-expo-appสร้างเฉพาะโครงในmobile/แบบno-installแล้วเลือกdependenciesbaselineก่อนinstall
เตรียมscripts/build-android.ps1และplugins/with-short-cmake-path.cjsตามREADME
ตั้งpackageของกลุ่ม/light/portrait/keyboardresize โหลดIonicons+MaterialCommunityIconsก่อนrender
อย่าอ้างว่า MQTT/SQLite ทำงานจนกว่าจะทำ Phase นั้น
รัน typecheck/lint ตรวจหน้าจอ360dp แล้วรายงานไฟล์ที่เปลี่ยนและสิ่งที่ยังไม่ทดสอบ
```

## P2 — ข้อมูลบนโทรศัพท์

```text
ทำ Phase 2 ตาม data-model.md ใช้ expo-sqlite เป็นแหล่งข้อมูลถาวรหลัก
ทำ migration, repository, transaction, CRUD, duplicate/max5 validation,
history/retention และ error states ไม่เพิ่ม cloud database/login
เปิดใหม่ให้ข้อมูลกลับแต่ connection/freshness/pending ต้องไม่กลายเป็น live อัตโนมัติ
ทดสอบ T03 T04 T05 T14 และบันทึกผลจริง ไม่แก้ requirements เพื่อให้โค้ดที่ไม่ตรงผ่าน
```

## P3 — เชื่อม MQTT

```text
ทำ Phase 3 ตาม mqtt-contract.md ใช้ MQTT.js ใน React Native ผ่าน
wss://broker.emqx.io:8084/mqtt, MQTT3.1.1, clientIdไม่ซ้ำ
หนึ่งclient/listenerชุดเดียว, subscribeเฉพาะprofiles, QoS0, clean session,
queueQoSZero=false, ไม่มีcommand replay และไม่ปิดTLS validation
ใช้forceNativeWebSocket=true/manualConnect=true/resubscribe=false
reconnectPeriod2000/connectTimeout12000/keepalive15และservice subscribe4topicsต่อdevice
sensorfreshnessแยกจากboardpresence; stale client callbackต้องไม่เปลี่ยนstateของsessionใหม่
ตรวจ import/runtime ด้วยตัวอย่าง React Native ทางการให้เข้ากับ Expo รุ่นที่ใช้
แยกbroker connected, ESP32presence, cached/retained/live, sensorvalidity
รัน T06 T07 T08 T12 T13 และระบุผล Android จริงกับ mock แยกกัน
```

## P4 — Monitor และควบคุม Relay

```text
ทำ Phase 4: monitor temp/hum/valid, local timestamp/history และ reported Relay state
ส่งเฉพาะ ON/OFF ไป /set, retain=false, ไม่เปลี่ยนreportedจากปุ่มทันที
ทำ pending, callbackerror, matchedreport, timeout8s และ interrupted
ปุ่มปิดทั้งหมดเป็นสองคำสั่งผลแยกช่อง offline/staleต้องdisabled ไม่queue
ทดสอบ T09 T10 T11 และทวนว่า source ของstateมาจาก /state จริง
```

## P5 — ตรวจทั้งระบบและเก็บหลักฐาน

```text
ทำ Phase 5a/5b และtest-plan.md สรุปenvironment/versions/lockfile/คำสั่งที่รัน
รัน typecheck/lint/unit/integration และ Android runtime ที่ทำได้
เตรียม TEST_RESULTS.md ทุกIDเป็นผลจริงหรือยังไม่ทดสอบ
ทดสอบ H01-H12 กับผู้เรียน/ผู้สอนเมื่อมีบอร์ด ห้ามแต่งผล hardware
จับภาพสี่หน้าจอจริงและแยกจากภาพAI ปิดข้อมูลส่วนตัวในหลักฐาน
รายงาน APP-xx ที่ผ่าน/ยังไม่ผ่าน พร้อมไฟล์และเหตุผล ก่อนให้ผู้เรียนตรวจ diff/commit
ผลbare-boardไม่รับรองsensor/OLED/หน้าสัมผัสจริง และไม่อ้างว่ารันOpenCodeของผู้สอนมาแล้ว
```

## P-FW — สร้าง/ตรวจ firmware ให้ตรงแอป

```text
อ่านFW-01–10และmqtt-contract.md ตรวจBOARD_TARGET/Flash/PSRAM/portจากอุปกรณ์จริง
ถ้าใช้firmwareคอร์สที่มีอยู่แล้วให้ตรวจcontractและpinก่อน ไม่เขียนทับโดยไม่จำเป็น
หากต้องสร้างใหม่ให้แยกfirmware/lab07_smart_home/lab07_smart_home.ino,
firmware/lab07_smart_home/config.h และfirmware/lab07_smart_home/secrets.example.h
สร้างsecrets.hเฉพาะเครื่องและignore ไม่เอาcredentialsใส่MD/app/commit
รองรับArduino-ESP32core3.x; mappingESP32กับS3ต้องแยกตามtargetตามตาราง
คงtimers/activeLOW/resetOFF/networkhold/sensorinvalid/noOLEDcontinueทั้งหมด
MQTTTCP1883: exactsetสองหัวข้อ,reportedretain,status/LWT,sensorJSONvalid/null/uptime/age_s
ใช้clientIdไม่ซ้ำ ไม่เปลี่ยนON/OFFเป็นJSONcommand ไม่เพิ่มauto/OTA/provisioning
compiletargetที่ใช้และบันทึกผลจริง uploadเฉพาะบอร์ด/โหลดDCที่ผู้สอนระบุ
กรณีไม่มีhardwareให้รายงานcompile-only ไม่อ้างว่าหน้าสัมผัสทำงาน
```

## P-MX — ทำ MQTTX preset และคู่มือให้กลุ่ม

```text
ทำMX-01–06จากmqtt-contract.md ใช้assets/mqttx-monitor.template.json
สร้างmqttx-monitor.jsonแทนREPLACE-STUDENT-IDทั้ง6ตำแหน่งด้วยSTUDENT_IDของกลุ่ม
ใช้MQTT3.1.1/TCP1883/clean/keepalive30/reconnect4s/clientIdWithTime
alias01..06และสี#15803D,#2563EB,#B45309,#7C3AEDตรงตาราง ไม่ใช้wildcardซ้อน
messagesว่าง publishtopic/payloadว่าง QoS0 retainfalse ไม่มีtimedpublish/secret
เขียนขั้นตอนImportJSON→Connect→Plaintext→filterที่พื้นที่QoS0→เทียบset/state
ชื่อtopicเป็นจุดcopy สีเป็นหมวดไม่ใช่onlineflag ไม่มีphone/statusในprotocol
ตรวจในMQTTXเมื่อเปิดได้ ไม่เอาตัวจำลองมาเรียกว่าข้อมูลESP32จริง
```

## P-APK — Build และติดตั้งแบบที่ผู้สอนทดสอบ

```text
อ่านREADMEหัวข้อBuildAndroidบนWindowsและbaselineก่อนเริ่ม
ตรวจJDK17/SDK37/NDK27.1.12297006/CMake3.31.6/Ninja1.12.1ตามbuildจริง
ใช้buildhelperทำสำเนาpathสั้น ไม่ลบ/ย้ายsource ไม่แก้generatedGradleที่prebuildจะทับ
ลงconfigpluginและCMAKE_OBJECT_PATH_MAX240 ใช้packageของกลุ่ม
รันtypecheck/lint/tests/exportแล้วassembleRelease arm64-v8a,x86_64ที่รวมHermes/fonts
ติดตั้งAPKบนAndroidที่ได้รับอนุญาตด้วยinstall-r แล้วปิดMetroก่อนทดสอบ
หากsignature/packageไม่ตรงห้ามuninstallเอง ถ้ายังไม่มีphone/emulatorให้ระบุยังไม่ทดสอบ
ตรวจupgrade/localpersistence/รูป4หน้า/ข้อมูลinvalidและfreshnessตามtest-plan
บันทึกsourcecommit,APKversion/hash/signingประเภทtestหรือproductionและผลจริง
ห้ามบังคับให้hashAPKที่buildใหม่เท่ากับไฟล์ผู้สอนซึ่งtimestamp/keyอาจต่างกัน
```

## P6 — เมื่อพบปัญหา

```text
ปัญหาที่พบ: <อาการ/ข้อความerror/ขั้นตอนทำซ้ำ>
expected: <สิ่งที่ควรเกิดจากAPP-xx หรือtestID>
actual: <สิ่งที่เกิดจริง>
ตรวจสาเหตุจากโค้ดและเอกสารทางการก่อนแก้ แก้เฉพาะส่วนที่เกี่ยวข้อง
ห้ามแก้สเปก ลดtest ปลอมผล หรือข้าม validation เพื่อให้ผ่าน
รันทดสอบที่พิสูจน์ปัญหานี้และตรวจผลกระทบที่เกี่ยวข้อง แล้วอธิบาย diff แบบสั้น
```

## จุดที่ผู้เรียนต้องอธิบายเอง

- ทำไมการเพิ่ม profile ไม่ทำให้ ESP32 รู้รหัส Wi-Fi?
- profile/history อยู่ที่ใดหลังปิดแอป และอะไรหายไปเมื่อ uninstall?
- เหตุใดเห็น Broker connected แล้วจึงยังอาจสั่ง Relay ไม่ได้?
- ข้อความ state ที่ตรงกับค่าที่กดรับรองสิ่งใดได้ และยังรับรองอะไรไม่ได้?
- ถ้าพบ error จะส่งหลักฐานอะไรให้ AI แก้ได้ตรงเรื่อง?

คำถามเหล่านี้เป็นจุดสนทนาระหว่างทำโครงงาน ไม่ใส่คำตอบสำเร็จในไฟล์นี้

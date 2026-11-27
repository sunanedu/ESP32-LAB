# Data Model — ข้อมูลหลักอยู่บนโทรศัพท์

ใช้ SQLite ผ่าน expo-sqlite ตั้ง schema version/migration และ foreign keys ตาม API ของ SDK ที่เลือก ไม่ใช้ broker retained message เป็นประวัติถาวร และไม่เพิ่ม cloud database เพื่อแทนงานในเอกสารนี้

## Baseline schema2 ของแอป1.0.1

ใช้ `PRAGMA user_version` เป็นรุ่นจริง: 0→1สร้างsettings/devices, 1→2เพิ่มsamples/snapshots/commands/eventsและindexes เก็บprofileเดิมไว้ ถ้าพบDBรุ่นใหม่กว่าแอปให้แจ้งอัปเดต ไม่ล้างDB ตั้งWAL/foreign_keysก่อนใช้งาน และserializewriteผ่านคิวกับexclusive transactionเพื่อไม่ให้UI/MQTTแทรกtransactionกัน

คงรูปข้อมูลนี้เป็นสัญญาระหว่างRepository/Controllerไม่ผูกReact Native componentเข้ากับSQLโดยตรง `expo-sqlite` nativeและSQLite testadapterใช้interface runAsync/getAllAsync/withExclusiveTransactionAsyncที่ตรวจerrorได้

## ตารางที่ต้องมี

| ตาราง | คอลัมน์หลัก | กติกา |
|---|---|---|
| app_settings | id=1, schemaVersion, host, port, path, autoConnect, installationId, updatedAt | หนึ่งแถว, ค่า public endpoint เริ่มต้น, installationId สุ่มและคงเดิมข้ามเปิดแอป |
| devices | id local UUID, studentId, name, room, iconKey, relay1Label, relay2Label, createdAt, updatedAt | studentId unique แบบแยกตัวพิมพ์, สูงสุด 5, prefix คำนวณจาก studentId ไม่เก็บสำเนาที่ขัดกัน |
| sensor_samples | id, deviceId FK, receivedAt, temp nullable, hum nullable, valid, uptime, ageSeconds nullable | เก็บรอบ valid/invalid ที่ schema ถูกต้องพร้อม phone receive time |
| relay_snapshots | deviceId FK, channel=1/2, reported=ON/OFF, receivedAt, wasRetained | unique(deviceId,channel); ยังไม่เคยรับคือไม่มีแถว ไม่ใช่แถวONปลอม; restartไม่ใช่live |
| commands | id local UUID, deviceId FK, channel, desired, requestedAt, completedAt nullable, outcome | outcome: pending / publish_failed / reported_match / timeout / interrupted |
| device_events | id, deviceId FK, kind, summary, receivedAt | connection/state-change/parser-error แบบจำกัดจำนวน ไม่เก็บ packet ดิบไม่จำกัด |

createdAt/receivedAt เป็นเวลาจากโทรศัพท์ในรูปแบบเดียวกัน เช่น UTC ISO 8601 ก่อนแสดงแปลงตาม locale/timezone ของโทรศัพท์ ไม่ใช้ uptime มาแปลงเป็นวันที่โดยตรง เก็บ valid เป็นชนิดที่ SQLite รองรับแล้ว map กลับ boolean ใน domain model

## ข้อมูลที่ไม่ persist เป็นสถานะใช้งานจริง

- MQTT client/socket, connection state, subscription acknowledgments
- timer/lastLiveSeen ของ session, pending spinner ของ UI
- ค่าที่ผู้ใช้กำลังพิมพ์แต่ยังไม่บันทึก
- สิทธิ์สั่งงาน canSend และ “ESP32 online”

บันทึก pending command เพื่อมีประวัติได้ แต่เมื่อเริ่ม session ใหม่ให้เปลี่ยน pending ที่ค้างจากการปิดแอปเป็น interrupted ใน transaction ห้าม replay หรือ resume การส่ง

## เงื่อนไขเขียนข้อมูล

1. เปิด DB/migrate ให้พร้อมก่อน hydrate UI และเริ่ม MQTT
2. Add/Edit ใช้ validate ทั้ง form และ repository แก้ field ที่อนุญาตเท่านั้น ไม่ publish
3. Transaction ใช้กับลบ device+ประวัติ/clear history และการจบ command ที่เกี่ยวข้อง
4. ใช้ prepared/parameterized statements ห้ามประกอบ SQL จากชื่ออุปกรณ์/ข้อความที่ได้รับ
5. Parser error ไม่เขียน sensor sample; เก็บ event แบบ rate-limit เพื่อไม่ให้ external payload ทำ DB โตไม่หยุด
6. State ค่าเดิมที่ firmware ส่งเป็นระยะอัปเดต snapshot/เวลา แต่ event สร้างเฉพาะเปลี่ยนค่า
7. DB write error ต้องแจ้งผู้ใช้และ log แบบไม่ใส่ secret ไม่สรุปว่าบันทึกสำเร็จ และไม่ใช้วิธีล้าง DB ทิ้งอัตโนมัติ

## Retention

| ข้อมูล | ขีดจำกัด |
|---|---|
| sensor_samples | ล่าสุด 10,000 แถวต่อ device และอายุไม่เกิน 7 วัน ลบเมื่อถึงขีดใดก่อน |
| commands | ล่าสุด 1,000 แถวต่อ device |
| device_events | ล่าสุด 500 แถวต่อ device |
| relay_snapshots / devices / app_settings | คงไว้จนแก้ไขหรือลบโดยผู้ใช้ |

ทำ cleanup เป็นชุดหลัง insert ตามนโยบาย ไม่ scan ทั้ง DB ทุก render สร้าง index อย่างน้อย `(deviceId, receivedAt)` ในตารางประวัติ ขีดจำกัด 10,000 ไม่ได้แปลว่าเก็บครบ 7 วัน เพราะจำนวนข้อความอาจถึงก่อน

“ล้างประวัติ” ลบ samples/commands/events และทำ pending ที่เกี่ยวข้องให้จบก่อน ไม่ลบ profiles/settings; snapshots ล่าสุดเก็บเป็น cache พร้อมเวลาเหมือนเดิม “ลบอุปกรณ์” ลบ profile และข้อมูลที่อ้างถึงทั้งหมด หยุด subscription และไม่ส่ง MQTT เปลี่ยนโหลด

ปุ่มล้างประวัติในSettingsครอบคลุมทุกprofile ไม่ใช่เฉพาะบอร์ดที่เลือก แจ้งขอบเขตนี้ในconfirm ล้างเสร็จhistory/countต้องสะท้อนDBที่ว่าง ข้อมูลsensorในmemoryอาจยังเป็นค่าที่เพิ่งรับได้แต่ไม่ใช่แถวประวัติที่ยังอยู่ ส่วนrelay snapshotsคงไว้ เมื่อรับsensorรอบใหม่DBจึงเริ่มเก็บใหม่

ที่คาบ5s จำนวน10,000แถวจะถึงในประมาณ13ชั่วโมง53นาทีหากแอปรับต่อเนื่อง จึงไม่รับรองประวัติครบ7วัน และขณะAndroidพักแอปตัวอย่างนี้ไม่ได้มีbackground serviceคอยเก็บข้อมูลแทน

## การอ่านและvalidationให้ตรงรุ่นอ้างอิง

- ชื่ออุปกรณ์1–40, ห้อง0–40, Relay label1–30ตัวอักษรหลังtrim; STUDENT_ID1–24ASCII `[A-Za-z0-9-]+`, uniqueแบบCOLLATE BINARYและimmutableเมื่อEdit
- อ่านdevicesตามcreatedAt/rowid; เริ่มเลือกprofileแรก ไม่ต้องเพิ่มคอลัมน์lastSelectedที่ไม่มีในschemaนี้
- History sheetอ่านsensorล่าสุด60, commandsล่าสุด50, eventsล่าสุด50แล้วรวมเรียงเวลาล่าสุด ไม่ใช่แสดงทุก10,000แถวพร้อมกัน การ์ดDashboardเลือกcommand/event3รายการล่าสุด
- pruneหลังinsert/finish/eventและตอนinitializeตามนโยบาย ไม่จำเป็นต้องมีdaemoncleanupทุกวินาทีขณะปิดแอป จึงไม่อ้างว่าข้อมูลเก่าถูกลบตรงเวลาแม้แอปไม่รัน
- sensorที่schemaผิดไม่เขียนsample; parser error eventจำกัดไม่ถี่กว่า30sต่อบอร์ด; stateค่าเดิมอัปเดตเวลาแต่ไม่สร้างstate-change eventซ้ำทุก10s
- IDของcommandsเป็นlocalUUIDสำหรับhistory ไม่ใส่ลงpayloadON/OFFและไม่ทำให้protocolมีcausalACK

## อ่านเมื่อ offline

Dashboard ใช้ labels/room จาก DB ค่า snapshot ต้องระบุเวลาและ cache badge ประวัติดูได้ใน bottom sheet แม้ไม่มีเน็ต sensor invalid เป็นแถวที่ temp/hum=null ไม่เอาแถวดีเก่ามาแทนแถวล่าสุดโดยไม่บอก

## การทดสอบ storage

- ใส่ชื่อภาษาไทย/อักขระ quote แล้วอ่านกลับตรง ไม่กลายเป็น SQL
- ปิดแอป/force-stop/เปิดใหม่ labels, settings, history คงเดิม แต่ controls ยังรอยืนยัน
- Duplicate studentId ถูก reject โดย DB constraint และแสดง error แบบอ่านรู้เรื่อง
- จำลอง migration จาก schema version ก่อนหน้าโดยข้อมูลยังอยู่
- จำลอง DB error แล้ว UI ไม่แสดงว่าบันทึกผ่าน
- เพิ่มข้อมูลเกินเพดานและขยับเวลาจำลอง ตรวจเฉพาะบอร์ดที่เกี่ยวข้องถูก prune
- ล้างประวัติไม่ลบชื่ออุปกรณ์ และลบ profile ไม่ส่ง MQTT

ข้อมูล local ไม่ใช่การรับประกันการเข้ารหัสเอง ไม่มี Wi-Fi password ใน DB หากต่อยอดให้ใช้ credentials ให้ใช้ secure storage แยกตาม [Expo SecureStore](https://docs.expo.dev/versions/latest/sdk/securestore/) และไม่ใส่ secret ในหลักฐาน/export

[Expo SQLite — เอกสารทางการ](https://docs.expo.dev/versions/latest/sdk/sqlite/)

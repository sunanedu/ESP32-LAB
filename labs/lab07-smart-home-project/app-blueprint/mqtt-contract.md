# MQTT Contract — แอปกับ ESP32

สัญญานี้ตรงกับ firmware `lab07_smart_home` ของคอร์ส ผู้เรียนสร้างแอปตามสัญญานี้ ไม่ต้องปรับ firmware เพื่อให้ตรงกับ library/UI ที่ AI สุ่มเลือก

อ่าน [บทเรียนพร้อมโค้ดเต็มและคำสั่ง compile/upload](https://github.com/sunanedu/ESP32-LAB/blob/main/labs/lab07-smart-home-project/README.md) หรือดาวน์โหลด [โฟลเดอร์ firmware อ้างอิงของรุ่น1.0.1](https://github.com/sunanedu/ESP32-LAB/blob/main/downloads/README.md) ซึ่งมี OLED test, DHT test และ smart_home แยกกัน หากให้ OpenCode สร้างใหม่ด้วย P-FW ต้องคงเงื่อนไขด้านล่างและทดสอบใหม่ ไม่ใช้ Wi-Fi credentials ของผู้สอน

## FW-01–10 — ข้อกำหนดเมื่อสร้างหรือแก้ firmware

| ID | เงื่อนไขที่ต้องคงไว้ |
|---|---|
| FW-01 | Arduino-ESP32 core3.x; รอบอ้างอิง3.3.13, Serial115200, codeแยก .ino/config.h/secrets.example.h และsecrets.hเฉพาะเครื่อง |
| FW-02 | ตรวจtargetก่อนเลือกpin; ESP32-WROOM: DHT4,Relay26/27,I2C21/22; S3: DHT4,Relay16/17,I2C8/9; targetอื่นต้องตรวจmappingก่อน ไม่fallbackใช้ขาผิด |
| FW-03 | Relay activeLOW: ON=LOW/OFF=HIGH, resetเริ่มOFFทั้งคู่ แต่firmwareอย่างเดียวไม่รับประกันไม่กระตุกก่อนsetup; ต้องตรวจโมดูลรองรับ3.3Vและใช้โหลดLED DCเท่านั้น |
| FW-04 | บอร์ดต่อWi-Fi2.4GHz, TCP broker.emqx.io:1883, clientIdประกอบSTUDENT_IDและMAC48บิต ไม่ใช้ClientIDเดียวกับแอป/MQTTX |
| FW-05 | อ่านDHT11ทุกประมาณ5s, OLEDทุก1s, reported stateทุก10sและหลังcommand/connect; mqtt.loopถี่ ไม่มีdelayยาวในloop |
| FW-06 | ตรวจOLED ACK0x3Cก่อนinit; ไม่มีจอแล้วทำงานต่อ DHTNaNส่งtemp/hum=null,valid=false ไม่ส่งNaNหรือแปลงเป็น0 |
| FW-07 | เก็บlastValidRead/hasValidReading; age_sเป็นnullก่อนเคยได้ค่าดี, เป็นอายุข้อมูลดีล่าสุดภายหลัง; OLEDเก็บค่าดีเดิมพร้อมSTALE ส่วนMQTTรอบinvalidส่งnull |
| FW-08 | รับเฉพาะsetสองtopicของตน payloadไม่เกิน8byteแล้วtrim/uppercaseและยอมรับON/OFF; appส่งรูปแบบcanonicalตัวใหญ่ ไม่เปลี่ยนcontractเป็นJSONcommandเอง |
| FW-09 | เมื่อconnect: online retain + subscribeใหม่ + publishstate; LWT offline QoS0 retain, keepalive15s; PubSubClientpublishได้QoS0; MQTTbuffer256byte/JSONbuffer160byteตามsourceอ้างอิง |
| FW-10 | Wi-Fi retry10s/MQTT retry5s, socket timeout2s; networkหลุดคงRelayล่าสุด, sensorinvalidไม่สั่งRelay, resetกลับOFF ไม่เพิ่มauto-mode/OTAเอง |

`WiFi.reconnect`/`mqtt.connect` เป็นงานเครือข่ายที่อาจรอ แม้เว้นรอบด้วยmillisและตั้งsocket timeoutแล้ว นี่เป็นcooperative loop ไม่ใช่hard real-time ห้ามอ้างว่าทุกฟังก์ชันnon-blockingหรือคาบsensorตรง5sเสมอ

สำหรับ S3ที่ตรวจจริง Flash16MB/Octal PSRAM8MB ผ่านCH343ใช้ FQBNต่อไปนี้ เปลี่ยนFlash/PSRAMให้ตรงบอร์ดอื่นก่อนcompile:

```text
esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi,CDCOnBoot=default,UploadSpeed=115200
```

ESP32รุ่นเดิมใช้ `esp32:esp32:esp32` สำหรับS3ไม่มีGPIO22, GPIO26–32เกี่ยวข้องกับFlash/PSRAM และOctal memoryยังใช้33–37 จึงไม่ย้ายสายตามเลขขาESP32รุ่นเดิม ใช้USB CDC On Bootปิดสำหรับพอร์ตCH343 UART; nativeUSBเป็นอีกเส้นทางที่ต้องตรวจSerial/portใหม่

ไลบรารีรอบอ้างอิง: PubSubClient2.8.0, ArduinoJson7.4.3, Adafruit SSD1306 2.5.17, Adafruit GFX2.0.0, BusIO1.17.4, DHT1.4.7 และAdafruit Unified Sensor1.1.15 ค่าWi-Fiอยู่ที่firmwareเท่านั้น ไม่มีในMDที่จะส่งให้AI/แอป/ภาพหลักฐาน

หากใช้เฉพาะบอร์ดเปล่า ให้คาดหวัง OLED no ACK และsensorinvalidตามFW-06 ขณะที่state/คำสั่ง GPIOยังทดสอบได้ ไม่สร้างค่าอุณหภูมิจำลองลงnamespaceบอร์ดจริงเพื่อให้Dashboardดูผ่าน

## Endpoint และ session

| ฝั่ง | ค่า |
|---|---|
| แอป React Native / MQTTX Web | `wss://broker.emqx.io:8084/mqtt` |
| MQTTX Desktop monitor | `mqtt://broker.emqx.io:1883`, MQTT3.1.1 |
| ESP32 | host `broker.emqx.io`, TCP `1883` |
| MQTT version | 3.1.1 (`protocolVersion: 4` สำหรับ MQTT.js) |
| Authentication | public endpoint ไม่กำหนด username/password |
| Namespace ตัวอย่าง | `home/desk-a7f2` |
| Client ID แอป | `rn_` + installation ID สุ่มในเครื่อง; ไม่ซ้ำ ESP32/MQTTX |
| Clean session | true, subscribe ใหม่ทุก connect |
| Command QoS / retain | 0 / false |
| MQTT.js queueQoSZero | false เพื่อไม่สะสมคำสั่ง offline |
| Reconnect | เว้นประมาณ 2 s, client เดียว, ไม่สร้าง client ซ้อนทุก render |
| Timeout / keepalive ของแอป | connectTimeout12s / keepalive15s |
| React Native WebSocket | forceNativeWebSocket=true ใช้global nativeWebSocketของRN |
| Ownership | manualConnect=true, ติดlistenersก่อนconnect; resubscribe=falseแล้วservice subscribeต่อsessionหนึ่งชุด |
| Subscribe | สร้าง/คืน subscription ใน service จุดเดียว ไม่ทำทั้ง auto-resubscribe และ manual ซ้ำ |

ตรวจวิธี import/build สำหรับ React Native ของ MQTT.js รุ่นที่เลือกจากตัวอย่างทางการ ใช้ WSS ห้ามนำตัวอย่าง Node raw TCP มาใช้กับ Metro ตรงๆ แล้วแก้ด้วยการปิด TLS security

MQTTX เป็น client ดู/ส่งข้อความ มีหรือไม่มี MQTTX เปิดอยู่ แอปกับ ESP32 ก็ใช้ broker เดียวกันได้ public broker เป็นทางผ่าน ไม่ใช่ที่เก็บประวัติถาวรแทนฐานข้อมูลโทรศัพท์

## Topics และรูปแบบ payload

แทน `<id>` ด้วย STUDENT_ID จริง ตัวพิมพ์ตรง firmware ห้ามมี `/ + #` ใน ID

| Topic | ผู้ส่ง → ผู้รับ | Payload | Retain |
|---|---|---|---|
| `home/<id>/relay/1/set` | App/MQTTX → ESP32 | `ON` หรือ `OFF` | false |
| `home/<id>/relay/2/set` | App/MQTTX → ESP32 | `ON` หรือ `OFF` | false |
| `home/<id>/relay/1/state` | ESP32 → App/MQTTX | `ON` หรือ `OFF` | true |
| `home/<id>/relay/2/state` | ESP32 → App/MQTTX | `ON` หรือ `OFF` | true |
| `home/<id>/sensor` | ESP32 → App/MQTTX | JSON ด้านล่าง | false |
| `home/<id>/status` | ESP32/broker LWT → App | `online` หรือ `offline` | true |

แอป subscribe เฉพาะ state 2 ช่อง, sensor และ status ของแต่ละ profile รวมไม่เกิน 5 บอร์ด ห้าม subscribe `#` ทั้ง broker และไม่ต้อง subscribe `/set` เพื่อนำมาปลอมเป็น reported state

ตัวอย่างข้อมูลดี:

```json
{"temp":28,"hum":61,"valid":true,"uptime":35,"age_s":0}
```

ตัวอย่างรอบอ่านไม่ได้หลังเคยได้ค่าดี:

```json
{"temp":null,"hum":null,"valid":false,"uptime":40,"age_s":5}
```

ก่อนเคยมีค่าดี `age_s` เป็น null ได้ `uptime` คือวินาทีตั้งแต่บอร์ดเริ่ม ไม่ใช่ Unix timestamp firmware ส่ง sensor ทุกประมาณ 5 s และส่ง state ซ้ำทุกประมาณ 10 s เพิ่มจากตอนรับคำสั่ง/เชื่อมใหม่

## Parser ก่อนเปลี่ยน state/DB

1. ตรวจ exact topic จาก profile ที่มีอยู่ ข้าม packet ของ topic อื่น
2. จำกัด payload ไม่เกิน 1,024 byte; แปลง UTF-8 อย่างปลอดภัยและรับเฉพาะ schema
3. state รับเพียง `ON`/`OFF`; status รับเพียง `online`/`offline` แบบตรงตัว
4. sensor ต้องเป็น object, `valid` เป็น boolean, uptime เป็นจำนวนเต็มไม่ติดลบ, age_s เป็น null หรือจำนวนเต็มไม่ติดลบ
5. valid=true ต้องมี temp/hum เป็น finite number, ความชื้น 0–100; valid=false ต้องมี temp/hum=null
6. ปฏิเสธ JSON ผิดรูปแบบ/ชนิดผิด ไม่ crash ไม่ใช้ `Number(null)` เพื่อเปลี่ยนเป็นศูนย์ และไม่เขียนทับค่าดีล่าสุด
7. บันทึก receive timestamp จากโทรศัพท์ แยกเวลา runtime สำหรับ freshness ไม่เชื่อข้อความจาก broker ว่าเป็นคำสั่ง local ที่ผู้ใช้กด
8. ข้อมูล duplicate state ค่าเดิมอัปเดตเวลา freshness ได้ แต่ไม่ต้องสร้าง event เปลี่ยนสถานะซ้ำทุก 10 s

## เชื่อมต่อกับความสดของข้อมูลเป็นคนละเรื่อง

เก็บ `brokerState` (disconnected/connecting/connected/error), `devicePresence` (unknown/online/stale/offline), reported state 2 ช่อง และ sensor validity แยกกัน

เมื่อเปิดแอป/ต่อใหม่:
- Hydrate DB เป็น cache เท่านั้น ล้าง live-session markers และ pending เก่า
- ต่อ broker และ subscribe ของ profiles ที่มีอยู่ ต้องรอ subscribe สำเร็จก่อนเปิด controls
- Retained state/status ใช้แสดงค่าล่าสุดพร้อมป้าย “รอยืนยัน” แต่ **ไม่อัปเดต live freshness timer**
- Packet ที่ topic/schema ถูกต้องของ sensor หรือ state ซึ่ง **ไม่ใช่ retained replay** เป็น live evidence ของ session นี้ อัปเดต lastLiveSeen
- บอร์ดถือว่า online สำหรับการสั่งเมื่อ broker connected + subscriptions พร้อม + status ล่าสุดไม่ใช่ offline + มี live evidence อายุไม่เกิน 15 s และมี reported state ของช่องนั้นใน session นี้
- หลัง 15 s ไม่เห็น live evidence ให้ stale; เมื่อ status=offline ให้ offline ทันที ทั้งสองกรณีปิด controls
- ถ้า sensor valid=false แต่ packet schema ถูกต้อง แปลว่าบอร์ดยังส่งข้อมูลอยู่ได้ จึงไม่เท่ากับ MQTT offline; แสดง sensor invalid แยก
- เวลา runtime ใช้ monotonic timer/clock ที่ทดสอบได้ และ reset เมื่อกลับ foreground; Date/ISO ที่เก็บ DB ใช้เพื่อแสดง/ประวัติ ไม่ hydrate เป็น live timer

MQTT 3.1.1 ปกติ broker ส่ง retained replay พร้อม retain flag ส่วนข้อความส่งต่อสดให้ subscription ปัจจุบันไม่มี flag นั้น ให้ทดสอบ `packet.retain` กับ client รุ่นจริง ถ้าจำแนกไม่ได้ให้คง “รอยืนยัน” อย่าเดาว่าเป็นข้อมูล live

## หนึ่งคำสั่งจากปุ่ม power

1. ตรวจ canSend ของ profile/ช่อง; ถ้าปุ่ม disabled ไม่สร้าง command สำหรับส่งภายหลัง
2. ใช้ reported state ปัจจุบันเลือก desired ON หรือ OFF แล้วสร้าง local command record = pending
3. Publish ไป set topic ตรงช่อง QoS0 retain=false; ห้ามแก้ reported state ตามค่าที่กดทันที
4. ถ้า publish callback error → ส่งไม่สำเร็จ; ถ้าไม่ error ยังเพียง “ส่งออกแล้ว รอรายงาน”
5. เมื่อมี state แบบ live ไม่ใช่ retained replay ที่รับ **หลังเวลาส่ง** ตรง desired ใน session ปัจจุบัน → record reported_match และอัปเดต reported จาก packet
6. ถ้า state ไม่ตรงยังคงรอจน 8 s; ครบเวลา → timeout พร้อมข้อความไม่พบรายงาน ไม่ retry เอง
7. ถ้าหลุด/background/เปลี่ยน broker/ลบ profile ระหว่าง pending → interrupted; ไม่ส่งค้างใหม่หลัง reconnect
8. ปุ่มปิดทั้งหมดทำสอง command OFF แยกกัน ผลแต่ละช่องสำเร็จ/ล้มเหลวแยกกัน ห้ามรายงานว่าทำงานแบบ atomic

Protocol นี้ไม่มี commandId/reply correlation การเห็น state ตรง desired ไม่พิสูจน์ว่ามาจากคำสั่งนั้นโดยเฉพาะ และ firmware รายงาน GPIO ไม่ใช่ sensor ตรวจหน้าสัมผัส Relay

Timeoutไม่ได้ยืนยันว่าปลายทางไม่เคยสั่งGPIO อาจเป็นรายงานQoS0หายหรือมาช้า แอปห้ามrollback GPIO/ส่งOFFชดเชยหรือretryโดยอัตโนมัติ ให้แสดงstateล่าสุดที่ได้รับพร้อมผลcommandตามจริง commandUUIDในSQLiteเป็นIDประวัติlocal ไม่ใช่commandIdที่ถูกส่งบนwire

## MX-01–06 — MQTTX Desktop ที่ผู้เรียนต้องตั้ง

1. **MX-01 Namespace:** เลือกIDสมมติของกลุ่ม 1–24 ASCII `[A-Za-z0-9-]+` ตัวพิมพ์ต้องตรงfirmware/profile ไม่ใช้ `lab-s25-b884f3` ของผู้สอนในงานกลุ่มอื่น
2. **MX-02 Import:** ทำสำเนา `assets/mqttx-monitor.template.json` เป็น `mqttx-monitor.json` แทน `REPLACE-STUDENT-ID` ทั้ง6แห่งก่อนimport; กลุ่มStudent Monitor/TCP1883/MQTT3.1.1/Keepalive30s/reconnect4s/clientIdWithTime ไม่ใช่Wi-Fiของบอร์ด
3. **MX-03 Topics/colors:** สมัครexact6หัวข้อตามตาราง ไม่เปิด `home/id/#` ทับกัน ถ้าสร้างด้วยมือใส่alias/QoS0/colorทุกหัวข้อ
4. **MX-04 Display:** Light themeตัวอักษรเข้ม หน้ารวมPlaintext; คลิกพื้นที่แถบsubscriptionบริเวณQoS0เพื่อfilter กดซ้ำเพื่อล้าง การคลิกชื่อเป็นการcopytopic; กรองsensorก่อนเลือกJSONและคืนPlaintextเมื่อดูรวม
5. **MX-05 Read-only:** เว้นpublish areaว่าง ไม่มีtimed publish ถ้าผู้สอนจะสั่งเองใช้ON/OFF, QoS0,retain=false เฉพาะnamespaceตน ไม่publishstate/statusปลอมแทนบอร์ด
6. **MX-06 Interpret:** สีเป็นหมวดไม่เปลี่ยนตามpayload แม้offlineก็ขอบเขียว; ดูonlineล่าสุด+uptimeที่เพิ่ม MQTTXเขียวหมายถึงclientต่อbrokerได้ ไม่มีphone/statusในcontractนี้

| Alias | Topic ต่อท้าย `home/<id>/` | สี |
|---|---|---|
| 01 สถานะ · ESP32 | status | #15803D เขียว |
| 02 Sensor · DHT11 | sensor | #2563EB น้ำเงิน |
| 03 คำสั่ง · CH1 | relay/1/set | #B45309 ส้ม |
| 04 คำสั่ง · CH2 | relay/2/set | #B45309 ส้ม |
| 05 รายงานกลับ · CH1 | relay/1/state | #7C3AED ม่วง |
| 06 รายงานกลับ · CH2 | relay/2/state | #7C3AED ม่วง |

MQTTX1.13.3ที่ตรวจจริงimportgroup/connection/subscriptionsผ่านแล้ว หลังimportต้องConnectเอง การimportซ้ำIDเดิมอัปเดตentity/แทนsubscriptionsชุดนั้นและตัดconnectionsที่เปิดอยู่ ตรวจdiffไฟล์ก่อนนำเข้า ห้ามเก็บWi-Fi passwordหรือประวัติส่วนตัวในpreset

## กรณีทดสอบด้วย MQTTX

Connect WSS 8084 path /mqtt ด้วย Client ID สุ่มอีกค่า จากนั้น Subscribe `home/desk-a7f2/#` เพื่อ debug เฉพาะ namespace ตน:
- ส่ง `ON` ไป `/relay/1/set` แบบ retain ปิด ต้องเห็น `/relay/1/state=ON` จากบอร์ดและแอปเปลี่ยน reported state
- สั่งจากแอป ต้องเห็น set และ state คนละข้อความใน MQTTX
- ทดสอบ parser/freshness ด้วย fixtures ใน unit tests หรือ namespace ของบอร์ดทดสอบแยก ห้ามส่ง state/status จำลองไป prefix ของผู้อื่น
- หากเคยส่ง retained command ให้ล้างเฉพาะ topic ของตนก่อนเริ่มการทดสอบ แล้วปิด retain

## อ้างอิง

- [EMQX endpoint](https://www.emqx.com/en/mqtt/public-mqtt5-broker)
- [MQTT.js API และตัวอย่าง React Native](https://github.com/mqttjs/MQTT.js)
- [ESP32-S3 GPIO](https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/api-reference/peripherals/gpio.html)
- [MQTTX aliases/colors/format](https://mqttx.app/docs/get-started)
- [MQTTX import groups/conflicts](https://github.com/emqx/MQTTX/blob/main/docs/desktop/en/advanced/backup-and-restore.md)

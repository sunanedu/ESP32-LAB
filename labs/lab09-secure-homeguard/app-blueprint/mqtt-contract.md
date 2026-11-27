# MQTT และ key contract ของ Lab09

ใช้ [Level5 ข้อ 6–9](../../../Secure-IoT-Level-5.md) เป็นเกณฑ์เป้าหมาย รายละเอียด implementation/ตัวอย่าง ciphertext ที่ตรวจได้อยู่ใน [SYSTEM_WORKFLOW](../SYSTEM_WORKFLOW.md) หากสองส่วนต่างกันให้บันทึกตารางความต่าง ไม่ถือว่า source ผ่าน Level5 อัตโนมัติ

## Endpoint และ transport

| Profile | ปลายทาง | การตรวจรับ |
|---|---|---|
| public-lab | `broker.emqx.io:8883` TLS หรือ WSS8084 `/mqtt` | TLS/Session ตรวจได้ แต่ private ACL=N/A |
| private | host/port/path/CA ของบริการ; credentials โทรศัพท์/ESP แยก | ทดสอบ default-deny ACL และ wrong CA/hostname/time จริง |

MQTT3.1.1 clean session, retain=false ทุก secure topic ไม่มี offline control replay ของแอป TLS ตรวจ chain/hostname; SoftAP local HTTP ที่ `192.168.4.1` อยู่ภายใน Security2 ตาม network security config ไม่ใช่ข้ออนุญาต plaintext MQTT

## Topics

prefix = `iot/v1/<UUIDv4>/`; UUID ของบอร์ดไม่ใช่ STUDENT_ID/ชื่อ Broker

| suffix | ผู้ส่ง | เนื้อหาภายใน envelope |
|---|---|---|
| hello | phone | type, client_challenge |
| challenge | ESP | type, client_challenge, session_id, uptime_ms |
| cmd | phone | session_confirm / set_relay / get_state / renew_lease / commit_broker / rollback_broker |
| ack | ESP | type, request_id, status |
| state | ESP | type, relay1, relay2, state_version, uptime_ms |
| telemetry | ESP | type, uptime_ms, valid, temperature_c, humidity_pct |
| presence | ESP | type, uptime_ms |

## Envelope และกุญแจ

- root32 bytes/device/pairing จาก Native CSPRNG ไม่ใช้ Wi-Fi password/MAC/PIN และไม่ส่ง key ผ่าน JS
- HKDF-SHA256 output32 bytes: salt UUID16 bytes สำหรับ bootstrap, SID16 bytes สำหรับ Session; info แต่ละค่าเป็น `secure-iot/v1/bootstrap/c2d`, `secure-iot/v1/bootstrap/d2c`, `secure-iot/v1/session/c2d`, `secure-iot/v1/session/d2c`
- exact envelope fields `v,kid,sid,seq,nonce,ct,tag`; wire<=2048 bytes, plaintext<=1024; UTF-8/strict JSON/ไม่รับ duplicate fields
- `v=1`, `kid=1` ใน runtime นี้; sid เป็น hex lowercase32; seq เป็น canonical decimal **string**/uint64; nonce12/tag16 bytes เขียน Base64 standard padded
- AAD=`v1|kid|sid|seq|exactTopic`; Session nonce=zeros4+uint64BE(seq) เลขลำดับร่วมทุก Topic ต่อ directional key
- bootstrap sid=ศูนย์32/seq0/nonceสุ่ม96-bit มี durable usage budget; Hello challenge128-bit, pending10s, RTTมือถือไม่เกิน2s
- Confirm ต้องได้ ACK request ID ตรงและผ่าน tag ก่อน authenticated; reconnect/reboot/resume สร้าง Session ใหม่ สูงสุด24h/ประมาณ2^20ข้อความต่อทิศทาง

## คำสั่งและคำตอบ

JSON ต่อไปนี้เป็น plaintext **ภายใน** envelope ไม่ใช่สิ่งให้ส่งตรงจาก MQTTX:

```json
{"relay":1,"value":true,"type":"set_relay","request_id":"e4c0c189-8bc0-42d4-a071-7f3f8365af70","deadline_device_ms":"125000"}
```

```json
{"type":"ack","request_id":"e4c0c189-8bc0-42d4-a071-7f3f8365af70","status":"applied"}
```

```json
{"type":"state","relay1":true,"relay2":false,"state_version":"42","uptime_ms":"120050"}
```

Deadline อยู่ใน now..now+5000ms บน uptime บอร์ด และตรวจอีกครั้งก่อน GPIO; application ACK ผูก request ID ส่วน State บอก GPIO command state ไม่ยืนยันหน้าสัมผัส Relay และ MQTT PUBACK ไม่ยืนยันผลอุปกรณ์

Replay envelope เดิม seq<=high-water ถูกทิ้ง; authenticated message ใหม่ seq สูงขึ้นแต่ request ID ซ้ำใน cache32 ให้ตอบผลเดิมโดยไม่ actuate ซ้ำ ไม่รับประกัน ACK ซ้ำให้แพ็กเกต seq เดิม

หลังส่งแอปรอ8s ถ้าไม่พบผลให้ unknown และขอ State ไม่ retry ON เอง หลัง authentication ต้องมีข้อความ authenticated สดใน15s; renew Lease10s/หมดอายุ30s ข้อมูล sensor มี valid/null แยกจากการเชื่อมต่อ

## ความต่างที่ยังต้องจัดการ

| เรื่อง | เป้าหมายเดิม | source ปัจจุบัน |
|---|---|---|
| Telemetry QoS | 0 | 1 เช่นเดียวกับ publishWire อื่น |
| keepalive | 60s | phone60 / ESP30 |
| command rate | 10/s | aggregate packet filter20/s ไม่ใช่ acceptance ของ10/s |
| attempts/cooldown | ต้องมีฝั่งอุปกรณ์ | มี client cooldown3ครั้ง/30s; server-side gate ต้องตรวจเพิ่ม |

Status ที่ native รับ: applied/invalid_schema/expired/unauthorized/busy/safety_lock/invalid_relay/internal_error แต่ firmware ไม่ได้ส่งทุกค่าในทุก failure โดยเฉพาะ invalid tag/session/replay อาจทิ้งเงียบ ห้าม log plaintext/key เมื่อ verify ไม่ผ่าน

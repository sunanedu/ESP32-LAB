# Secure IoT Level 5 --- Implementation Specification

เวอร์ชัน 1.0 \| 27 พฤศจิกายน 2569 \| เอกสารสำหรับ AI Coding Agent

## 1. ภารกิจและขอบเขต

สร้างระบบควบคุมรีเลย์และอ่านเซ็นเซอร์ผ่าน MQTT โดย Samsung Galaxy S25 Ultra (Android, React Native + TypeScript) และ ESP32-S3 (ESP-IDF + FreeRTOS) ใช้ private MQTT cloud free tier เป็นตัวกลาง ไม่ต้องมี backend ประจำ ระบบต้องรักษาความลับและความถูกต้องของ payload แม้ broker อ่านหรือแก้ไขข้อความได้ โดยไม่ทำให้การควบคุมภายในบอร์ดหยุดชะงัก Level 5 เป็นชื่อสถาปัตยกรรมภายใน ไม่ใช่ใบรับรองมาตรฐาน

### คำสั่งสำหรับ Agent

-   อ่านเอกสารทั้งหมดและสำรวจ repository ก่อนแก้ไข หากมีระบบเดิมให้รักษาฟังก์ชันและสำรองการตั้งค่า ห้ามลบไฟล์ผู้ใช้
-   สร้าง PLAN.md, PROGRESS.md, DECISIONS.md และ TEST_REPORT.md บันทึกงานจริง พร้อม checklist และหลักฐาน build/test; ห้ามรายงานผ่านเมื่อยังไม่ได้ทดสอบ
-   ทำทีละ phase ให้ build ผ่านก่อนต่อไป ใช้ dependency ที่รองรับกันจริง pin version และ lockfile; ห้ามฝัง secret ใน source, log, APK หรือ Git
-   หากไม่ทราบรุ่นบอร์ด ขารีเลย์ active level เซ็นเซอร์ หรือพอร์ต serial ให้ใช้ config และ simulator; ขอผู้ใช้ยืนยันก่อนจ่ายไฟให้โหลดจริงหรือ burn eFuse
-   ห้ามสร้างบัญชีเสียเงิน เปิด billing หรือเปลี่ยน eFuse โดยไม่มีการอนุมัติ แยก development และ production

## 2. Architecture

``` mermaid
flowchart TD
 A[Android React Native UI] --> B[Native Kotlin Security + MQTT]
 B -->|TLS + AEAD payload| C[Private MQTT Broker]
 C -->|TLS + AEAD payload| D[ESP-MQTT Network Task]
 D --> E[Authentication + Replay Validator]
 E --> F[Bounded FreeRTOS Queue]
 F --> G[Relay and Local Safety Task]
 H[Sensor Task] --> I[Telemetry Encoder]
 I --> D
 G --> D
 B --- J[Android Keystore]
 E --- K[Encrypted NVS]
```

Broker ไม่ได้รับ application keys; TLS ป้องกันผู้ดักฟังระหว่างทาง ส่วน AES-GCM ป้องกันเนื้อหาจาก broker ด้วย Broker ยังเห็น topic, ขนาด, เวลา, IP และสามารถ drop/delay/reorder/flood ข้อความได้ ไม่รับประกัน availability หรือ real-time ผ่านอินเทอร์เน็ต

## 3. Threat model

  ---------------------------------------------------------------------------------------------------------------
  ภัย                                มาตรการ                                      ข้อจำกัด
  --------------------------------- -------------------------------------------- --------------------------------
  ดักฟัง Wi-Fi/Internet               TLS ตรวจ CA และ hostname                     broker ยังเห็น metadata

  broker หรือ client อื่นอ่าน payload   AES-256-GCM                                  endpoint ที่ถูกยึดอาจอ่านได้

  ปลอมคำสั่ง/แก้ topic                 AEAD tag + AAD + ACL                         ผู้ถือ key มีสิทธิ์ตาม key

  replay/retained/duplicate         session challenge, sequence, expiry, dedup   ต้องทดสอบ reboot

  ขโมยโทรศัพท์                        Keystore, biometric, physical revoke         เครื่องที่ปลดล็อกแล้วมีความเสี่ยง

  อ่าน flash/firmware ปลอม           flash encryption, secure boot, signed OTA    ไม่รับประกันต่อ invasive attack

  DoS/broker ล่ม                     bounded queue, rate limit, local fail-safe   ไม่สามารถรับประกัน remote control
  ---------------------------------------------------------------------------------------------------------------

## 4. Stack และการตัดสินใจ

-   Android: React Native + TypeScript, native Kotlin module สำหรับ crypto, Keystore, BLE provisioning และ MQTT (เลือก MQTT client ที่ยังดูแลและรองรับ TLS/QoS1 เช่น HiveMQ MQTT Client หากเข้ากันกับ Android build; ทดสอบก่อนล็อก dependency). UI ไม่ถือ raw key ใน JS; ใช้ Android BiometricPrompt สำหรับ unlock/pair/revoke และคำสั่งเสี่ยงตาม policy
-   ESP32-S3: ESP-IDF stable ที่รองรับบอร์ด, ESP-MQTT, mbedTLS GCM/HKDF, FreeRTOS, NVS encryption, ESP provisioning, OTA rollback. ตรวจ sdkconfig และ hardware acceleration จริง ไม่สมมติว่า GCM ทุกส่วนใช้ hardware
-   MQTT 3.1.1 baseline เพื่อ compatibility; MQTT 5 optional (message expiry เสริม ไม่แทน application expiry)
-   EMQX Cloud Serverless เป็น candidate; ตรวจ free tier, ACL, TLS, region, limits, billing ณ วันติดตั้งจากเอกสารผู้ให้บริการ ห้ามใช้ public anonymous broker ใน production
-   V1 รองรับโทรศัพท์เจ้าของหนึ่งเครื่องต่อ ESP32 หลายตัว; multi-owner เป็นงานต่อยอด ต้องมี key/ACL/revocation แยกผู้ใช้

## 5. Repository ที่เสนอ

``` text
secure-iot/
  README.md
  AGENTS.md
  PLAN.md
  PROGRESS.md
  DECISIONS.md
  TEST_REPORT.md
  .gitignore
  docs/{architecture,protocol,threat-model,provisioning,broker,ota,recovery,hardware}.md
  protocol/{schema.json,vectors.json,generate_vectors.py}
  mobile/
    src/{screens,components,devices,mqtt,state,types,tests}/
    android/app/src/main/java/.../security/
      CryptoModule.kt
      KeyStoreManager.kt
      ProvisioningManager.kt
      MqttManager.kt
      SessionManager.kt
    .env.example
  firmware/
    CMakeLists.txt
    sdkconfig.defaults
    sdkconfig.defaults.dev
    sdkconfig.defaults.prod
    partitions.csv
    main/{app_main.c,board_config.h}
    components/{network,secure_protocol,session,provisioning,relay,sensors,storage,safety,ota}/
    test/
  infra/{acl-example.md,broker-setup.md}
  tools/{simulator.py,benchmark.py,security_tests.py}
  .github/workflows/ci.yml
```

ใช้ไฟล์แยกจริงตามความเหมาะสม ไม่สร้าง directory เปล่าเพื่อให้ดูครบ

## 6. MQTT contract

Device ID = UUIDv4 สุ่ม ไม่ใช่ secret; client ID แยก phone/device และไม่ชนกัน; topic ไม่มีชื่อบุคคลหรือข้อมูลเซ็นเซอร์

  -----------------------------------------------------------------------------------------------
  Topic prefix `iot/v1/<deviceId>/`   Publisher      Subscriber     QoS            Retain
  ----------------------------------- -------------- -------------- -------------- --------------
  hello                               phone          device         1              false

  challenge                           device         phone          1              false

  cmd                                 phone          device         1              false

  ack                                 device         phone          1              false

  state                               device         phone          1              false

  telemetry                           device         phone          0              false

  presence                            device         phone          1              false
  -----------------------------------------------------------------------------------------------

ใช้ credential สุ่มแยกแต่ละ client; ACL default deny, exact topic + direction, ห้าม wildcard ที่เปิดข้ามอุปกรณ์; ทดสอบบัญชีอื่นอ่าน/เขียนไม่ได้จริง ไม่พึ่ง client ID เพื่อ auth. ใช้ TLS 1.2+ ตรวจ CA/hostname ห้าม setInsecure; เวลา TLS ต้องถูกต้องและไม่ bypass หาก NTP ล้มเหลว. ตั้ง keepalive 60s, clean session true, reconnect exponential backoff 1--60s + jitter; ไม่ queue คำสั่งรีเลย์ขณะ offline. LWT เป็นเพียง hint ที่ไม่เชื่อถือสำหรับสถานะออนไลน์; UI ใช้ authenticated heartbeat และ timeout.

## 7. Cryptographic protocol v1 (normative)

### 7.1 Keys

สร้าง root key 32 bytes จาก CSPRNG ต่อ device/pairing ไม่ใช้ MAC address/password เป็น key. สร้าง directional keys ด้วย HKDF-SHA256: salt = UUID bytes 16 bytes, IKM=root, info ASCII `secure-iot/v1/bootstrap/c2d` หรือ `.../d2c` ตามทิศทาง, output 32 bytes. สำหรับ session keys ใช้ salt = sessionId bytes 16 bytes, IKM=root, info `secure-iot/v1/session/c2d` หรือ `.../d2c`. แยก key version และห้าม reuse root หลัง factory reset. AES-256-GCM, tag 16 bytes, nonce 12 bytes. ห้ามใช้ ECB/CBC แบบไม่มี authentication, ห้ามใช้ Base64 เป็น encryption.

### 7.2 Envelope

JSON UTF-8, maximum wire payload 2048 bytes; reject duplicate fields, unknown version, invalid lengths, noncanonical base64, invalid UUID, invalid topic. Base64 standard padded (RFC 4648). seq เป็น decimal string แบบไม่มี leading zero ยกเว้น 0; parse uint64 โดยไม่ใช้ JS Number.

``` json
{"v":1,"kid":1,"sid":"00000000000000000000000000000000","seq":"1","nonce":"<base64 12 bytes>","ct":"<base64 ciphertext>","tag":"<base64 16 bytes>"}
```

AAD = UTF-8 ASCII string `v1|<kid>|<sid>|<seq>|<exact MQTT topic>`; kid เป็น uint32 decimal canonical, sid lowercase hex 32 chars, topic ASCII exact. Envelope metadata ถูก authenticate ด้วย AAD; nonce ถูก authenticate โดย GCM เอง. ห้ามใช้ plaintext ก่อน verify tag สำเร็จ. จำกัด plaintext 1024 bytes, ตรวจ schema หลัง decrypt. ไม่ log plaintext หรือ key ใน production.

### 7.3 Bootstrap/session และ nonce

-   Bootstrap hello/challenge ใช้ sid เป็น 32 ตัวอักษรศูนย์, seq=0, bootstrap directional key และ nonce 96-bit จาก CSPRNG ใหม่ทุกข้อความ; จำกัดจำนวนข้อความต่อ key ไม่เกิน 2\^20 และ rotate root ก่อนถึงขีดจำกัด; เก็บตัวนับการใช้งานแบบ durable หรือ reserve block ก่อนใช้. ห้าม fallback RNG ที่ไม่ปลอดภัย. หาก retransmit ให้ส่ง ciphertext เดิม ไม่ encrypt ซ้ำด้วย nonce เดิม
-   Phone ส่ง encrypted hello มี clientChallenge สุ่ม 128-bit. ESP ตรวจ tag แล้วสร้าง pending sessionId สุ่ม 128-bit และ challenge ที่ echo clientChallenge พร้อม device uptime. Pending session มีอายุ 10s และจำนวนจำกัด; hello ไม่เปลี่ยน active session ทันที เพื่อไม่ให้ replay hello ยกเลิก session เดิม
-   Phone ตรวจ echo challenge ที่ยัง outstanding เท่านั้น จากนั้น derive session keys และส่ง encrypted `session_confirm` บน cmd. ESP ยืนยัน pending session และเปลี่ยน active session atomically หลังตรวจ tag/seq/expiry; ส่ง authenticated ACK. หาก ACK หาย ให้เริ่ม handshake ใหม่หรือ retry ciphertext เดิมตาม dedup policy
-   Session nonce = 4 zero bytes + uint64 sequence big-endian; seq เริ่ม 1 แยกแต่ละทิศทาง ใช้ counter เดียวร่วมทุก topic ภายใต้ directional key เดียวกัน. Serialize encryption/send ผ่าน worker เดียว. ห้าม reuse session หลัง reboot/reconnect/process restart; สร้าง session ใหม่เสมอ. Session อายุสูงสุด 24h หรือ 2\^20 messages ต่อทิศทาง แล้ว handshake ใหม่. Session ID ห้ามซ้ำ; RNG failure = fail closed
-   Receiver ใช้ high-water sequence ต่อทิศทาง; reject seq \<= last accepted (ยอมทิ้ง telemetry ที่มาช้า). QoS1 duplicate ที่ตรงกับ cached request อาจ resend ACK โดยไม่ actuate ซ้ำ; cache จำกัด 32 รายการ, ตรวจ tag ก่อน lookup; ACK ใหม่ใช้ sequence ใหม่. ห้าม reset counter ใน session เดิม

### 7.4 Freshness โดยไม่พึ่ง wall clock สำหรับคำสั่ง

Device challenge ส่ง monotonic uptime_ms; phone บันทึก monotonic send/receive time และประมาณ device uptime จาก challenge โดยใช้เวลาที่ผ่านไปตั้งแต่รับ challenge (อาจ underestimate ตาม latency). คำสั่งระบุ deadline_device_ms = estimated device uptime + 5000. Device reject หาก deadline \< now หรือ deadline \> now+5000; ตรวจซ้ำก่อน GPIO. ใช้ uint64 และกำหนดเป็น decimal string. หาก challenge RTT \> 2s หรือ session เก่า/clock drift จน reject ให้ handshake ใหม่. ไม่ใช้ wall clock ที่ผู้ใช้แก้ไขได้เพื่อ anti-replay. Session ใหม่ทุก reboot ทำให้ uptime reset ไม่เปิด replay ข้าม boot.

### 7.5 Plaintext examples

``` json
{"type":"set_relay","request_id":"<uuid>","relay":1,"value":true,"deadline_device_ms":"125000"}
```

``` json
{"type":"ack","request_id":"<uuid>","status":"applied","relay":1,"value":true,"state_version":"42"}
```

``` json
{"type":"telemetry","uptime_ms":"123000","temperature_c":26.4,"humidity_pct":58.1}
```

ใช้ set ON/OFF ไม่ใช้ toggle; request_id unique, relay index allowlist, value boolean เท่านั้น. ACK หมายถึง GPIO command applied ไม่ยืนยันหน้าสัมผัสรีเลย์จริงหากไม่มี feedback sensor. Error codes: invalid_schema, expired, unauthorized, busy, safety_lock, invalid_relay, internal_error. Invalid tag/unknown session drop เงียบและนับ metric ไม่ตอบ oracle. State version monotonic ภายใน boot/session; state snapshot หลัง handshake และหลังเปลี่ยนสถานะ; UI ไม่แสดง success ก่อน authenticated ACK/state. หาก timeout แสดง unknown แล้ว query state; ไม่ส่งคำสั่งใหม่อัตโนมัติ

## 8. Provisioning / Key storage / Rotation

-   ใช้ ESP-IDF BLE provisioning security scheme ที่ยืนยัน proof of possession และเข้ารหัส เช่น Security 2 (SRP6a + AES-GCM) โดยตรวจ compatibility ของ Android SDK จริง. QR มี device ID, BLE service identifier และ PoP สุ่มเฉพาะตัว ไม่เผย root key. PoP ต้องมี entropy สูงและส่งผ่านช่องทางกายภาพที่เชื่อถือได้
-   กดปุ่ม physical เปิด pairing 120s; จำกัด attempts และ cooldown; ห้าม remote enable pairing. ส่ง Wi-Fi config, broker credential, root key ผ่าน authenticated encrypted provisioning เท่านั้น. ปิด BLE หลังเสร็จ. ไม่ log QR/PoP/password
-   Android: เก็บ root key ที่ wrap ด้วย AES key แบบ non-exportable ใน Android Keystore; ciphertext เก็บ app-private storage, exclude backup; ตรวจ security level จริง. Native module unwrap เฉพาะเมื่อจำเป็นและลดอายุ raw key ใน RAM. Biometric gate สำหรับการเปิด control session ตาม policy; ไม่บังคับสแกนทุก telemetry. ห้าม AsyncStorage เก็บ secret
-   ESP: encrypted NVS + flash encryption production, root key แยกจาก broker password; zeroize temporary buffers. Factory reset ต้อง physical confirmation และตั้ง relay safe ก่อนลบ keys; revoke broker credentials ผ่าน dashboard
-   Rotation session อัตโนมัติทุก 24h; root rotation ทุก 30 วันเป็น policy ที่เสนอ โดย V1 ใช้ physical authenticated re-pairing (ไม่ส่ง root ใหม่ผ่าน key ที่สงสัยว่ารั่ว). แสดงเตือนล่วงหน้า; เปลี่ยน kid, revoke old root/credential, clear session. หากต้องการ remote root rotation ให้ทำ protocol แยกพร้อม atomic two-phase commit และ recovery; ห้าม implement แบบ ad hoc. โทรศัพท์หาย: physical reset/re-pair และ revoke credential เดิม. เก็บ recovery procedure ไม่เก็บ plaintext key backup

## 9. Firmware runtime และ safety

-   Network task จัดการ Wi-Fi/MQTT/TLS; crypto worker ตรวจ envelope/tag/session/sequence/schema แล้วส่ง validated command ไป bounded queue (เริ่ม 8 entries); relay task เป็นผู้เดียวที่เขียน GPIO. Sensor task sampling 100--500ms ตาม sensor; telemetry every 10s หรือ threshold change พร้อม rate cap. ใช้ event-driven ไม่ busy loop. Core 0 network, core 1 control เป็นแนวทางเริ่มต้น ปรับตาม profiling; ห้าม block MQTT callback
-   ตั้ง GPIO safe ก่อนเริ่ม network; active-low/high configurable, หลีกเลี่ยง boot strapping/USB/flash pins ตาม board schematic. ใช้ external pull resistor/driver ที่เหมาะสม; ทดสอบ boot glitch ด้วย logic analyzer. ห้ามต่อ mains โดยไม่มีการออกแบบ isolation/fuse/enclosure และผู้มีความชำนาญ
-   Default fail-safe = OFF สำหรับโหลดทดสอบ; กำหนด per-load policy ก่อน production เพราะ OFF ไม่ปลอดภัยสำหรับทุกโหลด. Remote lease default 30s (authenticated renewal ทุก 10s เมื่อ remote control active); หมด lease, watchdog, invalid configuration หรือ network loss ตาม policy ให้ safe state. Local automation แยก policy และไม่ขึ้นกับ broker. E-stop/physical override มี priority สูงสุด; ไม่ auto-restore ON หลัง reboot
-   จำกัด command 10/s ต่อ session, bounded parsing, max payload 2048, drop malformed, no dynamic unbounded allocation. Queue full =\> busy, ไม่ overwrite safety event. Watchdog และ brownout detection; telemetry queue coalesce/drop เมื่อ offline. Metrics: free heap/min heap, stack high water, queue depth, crypto time, reconnect, rejected messages; ไม่ log secret

## 10. Mobile UX

หน้าจอ onboarding/pairing, device list, dashboard relay + sensor, connection status, settings/security, diagnostic/export sanitized logs. สถานะ relay: unknown/pending/on/off/error; offline disable remote command; แสดง last authenticated update. แยก broker connected จาก device authenticated online. App background อาจถูก Android suspend; ห้ามรับประกัน monitoring ต่อเนื่อง; เมื่อ resume handshake ใหม่และ query state. ใช้ foreground service เฉพาะเมื่อจำเป็นและมี notification/permission ตาม Android. รองรับหลาย device ด้วย credential/key แยก; ไม่ส่งข้อมูลข้าม device

## 11. Secure boot, flash encryption, OTA

Development ใช้บอร์ดที่ยังไม่ burn eFuse. Production เปิด Secure Boot V2, flash encryption release mode, encrypted NVS, signed firmware, debug/download restrictions ตาม threat model และ ESP-IDF guide ของ chip revision. สร้าง partition table จาก flash size จริง มี otadata + ota_0/ota_1 + NVS และพื้นที่ key ตาม config; ตรวจ image fit. Signing private key เก็บ offline/CI protected secret ไม่อยู่ใน firmware หรือ repo. OTA ผ่าน HTTPS ตรวจ certificate และ signed image; version/anti-rollback policy, power-loss recovery, boot self-test แล้ว mark valid; rollback หาก fail. ทดสอบบนบอร์ดสำรองก่อน irreversible eFuse. Agent ต้องแสดง eFuse summary, ขั้นตอน recovery และขออนุมัติแยกก่อน burn; ห้าม execute production provisioning อัตโนมัติ

## 12. Performance / Free tier

เป้าหมายเบื้องต้น ไม่ใช่ผลวัดจริง: crypto encrypt/decrypt p95 \<=5ms ต่อ payload 256B บน ESP; local control tick \<=20ms; end-to-end command ACK p95 \<=500ms บน Wi-Fi ที่เสถียร (บันทึก region/RTT); ไม่มี watchdog reset ใน soak 24h; free heap เหลืออย่างน้อย 50KB หลัง TLS และไม่มีแนวโน้ม leak; ปรับตาม board และ stack จริง. วัด p50/p95/p99, CPU per core, heap, stack, battery phone, reconnect; เปรียบเทียบ baseline TLS-only กับ full security 1000 commands บนโหลดจำลอง. ห้ามลดการตรวจ tag/TLS เพื่อให้ผ่าน benchmark

ตัวอย่าง 1 ESP + 1 phone ออนไลน์ 30 วัน = 86,400 session-minutes. Telemetry 256B ทุก 10s = 259,200 messages, ingress+egress ประมาณ 132.7MB ต่อผู้รับหนึ่งราย ไม่รวม headers, ACK, lease, TLS, retransmit. ตรวจ quota จริงและตั้ง budget alert/usage cap หากรองรับ; ไม่เปิด overage โดยอัตโนมัติ. ใช้ CBOR หรือ binary envelope เป็น v2 หลังวัดจริง ไม่เปลี่ยน encoding เงียบ ๆ

## 13. Implementation phases / acceptance gates

-   [ ] P0 สำรวจ hardware/repo, pin versions, board config, broker plan, threat model; build skeleton ทั้งสองฝั่ง
-   [ ] P1 simulator + GPIO safe boot + local relay/sensor; unit test safety policy
-   [ ] P2 private broker TLS + ACL; ทดสอบ unauthorized client และ invalid cert
-   [ ] P3 crypto protocol + shared vectors; cross-language Kotlin/C/Python tests ผ่าน byte-for-byte
-   [ ] P4 session handshake, replay/expiry, ACK/state, reconnect; test reboot และ packet loss
-   [ ] P5 BLE pairing + encrypted storage + physical revoke; ตรวจไม่มี secrets ใน logs/APK/repo
-   [ ] P6 UI และ diagnostics; ทดสอบ S25 Ultra จริง, background/resume, Wi-Fi/mobile switch
-   [ ] P7 load/soak/fault injection, quota estimate, performance report; แก้ bottleneck
-   [ ] P8 signed OTA + rollback บน dev board; เตรียม production hardening; approval gate ก่อน eFuse
-   [ ] P9 production validation, handover, recovery drill และคู่มือภาษาไทย

## 14. Test matrix

  -------------------------------------------------------------------------------------------------------------------------------
  ID                      Test                                                        Expected
  ----------------------- ----------------------------------------------------------- -------------------------------------------
  T01                     AES-GCM known-answer + HKDF RFC vectors                     ตรง reference

  T02                     Kotlin encrypt -\> C decrypt และย้อนกลับ                      ตรง bytes/AAD

  T03                     flip ciphertext/tag/nonce/AAD/topic                         reject ไม่มี GPIO change

  T04                     replay command / duplicate QoS1                             ไม่ actuate ซ้ำ

  T05                     replay hello/challenge/confirm                              ไม่เปลี่ยน active session โดยไม่ได้ auth ใหม่

  T06                     reboot ESP/phone, reconnect                                 session/key ใหม่ ไม่รับ packet เก่า

  T07                     expired/future deadline, delayed queue                      reject ก่อน GPIO

  T08                     wrong device/key/kid, revoked phone                         reject

  T09                     invalid TLS cert/hostname, clock invalid                    fail closed

  T10                     wildcard subscribe/cross-device publish                     ACL deny

  T11                     malformed JSON, oversized, duplicate fields, seq overflow   bounded reject ไม่ crash

  T12                     flood, queue full, broker offline                           local control ทำงาน, fail-safe ตาม policy

  T13                     boot/brownout/watchdog                                      ไม่มี unintended ON

  T14                     pairing timeout/wrong PoP/repeated attempts                 reject/cooldown

  T15                     app background/kill/network switch                          UI unknown จน sync ใหม่

  T16                     OTA unsigned/corrupt/old image                              reject ตาม policy

  T17                     OTA power cut / self-test failure                           recover/rollback

  T18                     root rotation/reset                                         old key/credential ใช้ไม่ได้

  T19                     1000 commands + 24h soak                                    metrics ตามเป้าหมายหรือรายงาน deviation

  T20                     nonce audit across reboot/concurrency                       ไม่มี reuse ภายใต้ key เดียวกัน

  T21                     broker captures payload                                     อ่าน plaintext ไม่ได้, metadata documented

  T22                     secret scan + backup inspection                             ไม่มี raw secrets ใน artifact
  -------------------------------------------------------------------------------------------------------------------------------

ใช้ Python reference cryptography library สร้าง deterministic vectors ด้วย test keys เท่านั้น; include key/salt/info/AAD/nonce/plaintext/ciphertext/tag และ negative vectors. CI รัน lint, typecheck, unit, protocol vectors, firmware build, Android debug build, secret scan; hardware tests แยก manual gate. ห้ามใช้ test keys ใน production

## 15. Build / handover

Firmware: `idf.py set-target esp32s3`, `idf.py build`, `idf.py -p <PORT> flash monitor` เฉพาะ dev board ที่ยืนยันแล้ว. Mobile: install locked dependencies, Gradle debug build และ install ผ่าน adb หลังยืนยัน device. Agent ต้องระบุคำสั่งจริงตาม repo/version, prerequisites Windows/WSL และผล exit code. ไม่สมมติว่า COM8 หรือ USB device พร้อมใช้งาน

Deliverables: source ทั้งสองฝั่ง, protocol spec + vectors, broker ACL guide, wiring/config guide, build scripts, debug APK, dev firmware binaries, signed production release procedure (ไม่เผย private key), test report, performance report, recovery guide และ checklist. Definition of Done: P0--P9 ผ่านตาม scope ที่อนุมัติ; test ที่ยังไม่ได้รันต้องระบุ NOT RUN พร้อมเหตุผล; production release ห้ามอ้างว่าปลอดภัยสมบูรณ์โดยไม่มี independent review

## 16. Official references สำหรับ Agent ตรวจสอบก่อน implement

-   ESP-IDF ESP32-S3 security: https://docs.espressif.com/projects/esp-idf/en/stable/esp32s3/security/security.html
-   ESP-MQTT: https://docs.espressif.com/projects/esp-mqtt/en/latest/
-   Android Keystore: https://developer.android.com/privacy-and-security/keystore
-   Android biometric: https://developer.android.com/identity/sign-in/biometric-auth
-   EMQX Cloud: https://docs.emqx.com/en/cloud/latest/
-   HKDF RFC 5869: https://www.rfc-editor.org/rfc/rfc5869
-   GCM NIST SP 800-38D: https://csrc.nist.gov/pubs/sp/800/38/d/final
-   MQTT 3.1.1: https://docs.oasis-open.org/mqtt/mqtt/v3.1.1/mqtt-v3.1.1.html

เอกสารนี้เป็น specification ที่เสนอ ไม่ใช่ implementation ที่ผ่านการ audit; ยังไม่ได้ตรวจสอบ quota ปัจจุบันหรือทดสอบ hardware จริง. ให้ Agent ตรวจสอบรายละเอียด API, SDK, board และ protocol review ก่อนใช้งานจริง โดยเฉพาะระบบที่ควบคุมโหลดอันตราย

# ปัญหาที่พบ บทเรียน และวิธีป้องกันการเกิดซ้ำ

อัปเดต 27 พฤศจิกายน 2569 · [เริ่มสร้าง](BUILD_GUIDE.md) · [ตั้งค่า](CONFIGURATION.md) · [วิธีวิเคราะห์เดิม](../PAIRING_DIAGNOSIS.md)

สถานะ **แก้แล้ว** ด้านล่างอ้างอิง source/ผลเดิมใน [TEST_REPORT](../TEST_REPORT.md) ไม่ได้หมายถึงผ่าน acceptance ทุก transport หรือทุก fault case การแก้เอกสารรอบนี้ไม่ได้สร้างผล hardware ผ่านใหม่

## 1. ปัญหาที่มีหลักฐานและข้อแก้

| ID / สถานะ | อาการและสาเหตุที่พบ | ข้อแก้ในระบบ | สิ่งที่ต้องตรวจก่อนเกิดซ้ำ |
|---|---|---|---|
| I-01 แก้ integration แล้ว | GATT connected/discovery/MTU สำเร็จแต่ค้างก่อน SRP; `readDescriptor()` อยู่ใน `Log.d()` การตัด log ทำให้ side effect หาย | เรียกอ่าน descriptor โดยตรง ตรวจ return และ callback; `BleDiscoveryTest` จับบั๊กเดิมได้ | ลบ log โดยคง side effect; รัน discovery tests แล้วทดสอบบอร์ดจริง |
| I-02 แก้ใน 2.0.5 | สร้างแพ็กเกต SRP ไม่ได้ใน Release APK ทั้ง Samsung/emulator; R8 เปลี่ยน fields ที่ Protobuf Lite ใช้ตอนรัน | consumer rule ของ `espressif.**` และ self-test สร้าง/อ่าน SRP packet | JVM PASS ไม่พอ ต้องรัน self-test บน minified Release APK ก่อน hardware pairing |
| I-03 แก้ firmware แล้ว | BLE controller เคย panic ตอนเริ่ม; การจอง BLE memory ไม่ตรง pattern ของ Arduino core | include `esp32-hal-alloc-ble-mem.h` ตาม integration ของ core | ตรวจ core/API ที่ใช้จริง และเปิด/ปิด transport ซ้ำบนบอร์ด |
| I-04 แก้ lifecycle แล้ว | assert ระหว่างปิด provisioning จากการ deinit ซ้ำกับ WiFiGeneric | ขอ stop และรอ `ARDUINO_EVENT_PROV_END` ให้ core deinit จากจุดเดียว | ทดสอบสำเร็จ/หมดเวลา/cancel/เปิดรอบถัดไป แยกแต่ละกรณี |
| I-05 แก้อายุข้อมูลแล้ว | API ของ BLE เก็บ UUID pointer จนเริ่ม service แต่ตัวแปรเดิมอยู่ใน block อายุสั้น | ใช้ storage ที่อยู่ตลอดอายุ runtime | ตรวจ lifetime ของทุก buffer ที่ส่งให้ async SDK |
| I-06 มีข้อแก้; SoftAP ยังรอครบ workflow | Android Wi-Fi system dialog ทำ Activity pause โค้ดเดิมยกเลิก local pairing | คงเฉพาะ pending network dialog หยุด MQTT control และเริ่ม SRP เมื่อกลับ foreground | ทดสอบ SoftAP consent/deny/cancel/background จริง ไม่ใช้ผล BLE แทน |
| I-07 ปรับก่อนส่ง config | MTU256 ต้องเผื่อ JSON/Base64/tag; chunk ใหญ่เสี่ยงเกินขนาดที่ส่งได้ | custom bundle chunk64 bytes และ response/timeout bounds | รักษาการเข้ารหัสทุก chunk ไม่ลดความปลอดภัยเพื่อส่งให้ผ่าน; ตรวจ MTU ที่เจรจาจริง |
| I-08 แก้ endpoint แล้ว | รอบพัฒนาเคยมีภาระ stack สูงใน custom endpoint ของบอร์ด | ย้าย token buffer ขนาดคงที่ออกจาก stack ไป allocation ที่ตรวจ failure ได้ | ตรวจ stack high-water/heap และ oversize/parse failures; build PASS ไม่แทน soak |
| I-09 ปรับ UX หลายรอบ | ผู้ใช้หา action จับคู่ไม่พบ/ปุ่มอาจถูกซ่อนหลังเลื่อนฟอร์ม | รุ่นปัจจุบันใช้ปุ่มเล็กท้ายฟอร์มใน ScrollView เดียว | ทดสอบจอจริง คีย์บอร์ด การเลื่อนจนเห็นปุ่ม และไม่ให้เมนูบัง ไม่ยึดตำแหน่ง footer จากเอกสารรุ่นเก่า |
| I-10 ปรับ UI ถึง 2.1.3 | เมนูล่าง/ไฮไลต์/ภาพถูก crop ไม่ตรงที่ผู้ใช้ต้องการ | Compact Center 50dp, เปลี่ยนสีแท็บ, ปรับกรอบภาพตามสัดส่วน | ตรวจห้าแท็บและภาพบนจอจริง; การผ่าน UI ไม่ยืนยัน command workflow |
| I-11 workflow build ใช้พาธสั้น | Native/CMake บน Windows มีข้อจำกัดพาธและไฟล์ generated | build script + BuildRoot สั้น + CMake plugin ที่ติดตาม source | ตรวจ plugin ครบและทดสอบหลัง Prebuild จาก source ใหม่ ไม่แก้เฉพาะ cache |
| I-12 แก้เอกสารรอบนี้ | PLAN/PROGRESS/schema/contract บางแห่งยังบอกเพียง starter หรือใช้ค่าเป้าหมายเป็นค่าปัจจุบัน | มีสารบัญตามงาน คู่มือตั้งค่า ตารางความต่าง และสถานะล่าสุด | ทุกครั้งที่แก้พฤติกรรมให้ปรับ contract/guide/test-plan/status พร้อมกัน |

I-07 เป็นการป้องกันขนาดแพ็กเกตจากค่าที่สังเกต ไม่ใช่หลักฐานว่าทุกรอบที่ค้างไปถึงขั้นส่ง config แล้ว รายละเอียด log/รุ่น/ข้อจำกัดอ่านในรายงานต้นทาง

## 2. ประเด็นที่ยังเปิดอยู่

| ID | ช่องว่างที่ต้องทำต่อ | หลักฐานที่จะใช้ปิดงาน |
|---|---|---|
| O-01 | SoftAP Security2 ครบการส่ง config/proof/reconnect | Android + บอร์ดจริง: success/deny/cancel/timeout และรอบถัดไป |
| O-02 | ย้าย Broker และ recovery สองฝั่ง | stage/session/confirm/commit, timeout/reboot/ACK หาย, บัญชี/CA ผิด โดยไม่ค้างครึ่งทาง |
| O-03 | ความต่างจากเกณฑ์ต้นทาง | command limit 10/s เทียบ source filter20 packets/s; Telemetry QoS0 เทียบ1; keepalive60 เทียบESP30; ตัดสิน/ทดสอบโดยไม่ลดเกณฑ์เงียบ ๆ |
| O-04 | server-side attempts/cooldown และ revocation/rotation | ผลบอร์ดจริงแยกจาก client cooldown และแผน old-phone ใช้ไม่ได้ |
| O-05 | การป้องกันกุญแจบน endpoint | ESP encrypted NVS/Flash/Secure Boot; Android hardware/security-level และ key-use auth policy ตาม requirement |
| O-06 | อุปกรณ์/ความทนทานจริง | DHT/OLED/หน้าสัมผัส Relay/ปุ่ม18, reboot/lease/replay, 1000 commands/24h soak และ independent review |

## 3. วิธีไล่ปัญหาให้เร็วและไม่แก้ผิดชั้น

1. ระบุ **source commit + working-tree status, APK version/hash, firmware/core, transport**
2. ถ้า build ไม่ผ่าน ให้หยุดที่ build ไม่เปิด pairing เพื่อรอแก้ native compilation
3. ถ้า self-test ล้มบน release ให้แยก SDK/R8/packet generation ก่อนตรวจเครือข่าย
4. ถ้า BLE ไม่ผ่าน descriptor ให้ตรวจ GATT integration ก่อนเปลี่ยน Broker/Wi-Fi
5. ถ้า Security2 ผ่านแต่ config ไม่จบ ให้ตรวจ chunk, endpoint response/proof, memory และ window
6. ถ้าจับคู่แล้วแต่ Session ไม่ผ่าน ให้แยก TLS/MQTT connected, UUID/topics, challenge/confirm และ credential ของอุปกรณ์
7. ถ้า Session ผ่านแต่ command ไม่ผ่าน ให้ดู deadline/seq/request/Lease/safety lock แล้วอ่าน ACK/State ไม่ใช้ PUBACK เป็นผล GPIO
8. แก้หนึ่งสาเหตุ เพิ่ม regression ที่จับบั๊กเดิม แล้วทดสอบซ้ำเฉพาะ gate ที่ได้รับผลกระทบ บันทึกสิ่งที่ยัง NOT RUN

ห้ามแก้ด้วย Security0/1, plaintext MQTT, ปิดตรวจ CA/hostname, fake success หรือบันทึกกุญแจลง log เพื่อให้ตามงานง่าย

## 4. แบบบันทึกปัญหาที่ไม่ใส่ secret

```text
Issue ID / วันที่:
Source commit / working tree:
APK versionCode / SHA-256 / architecture:
Firmware/core / board / mode:
Transport / ขั้นตอนที่ทำ:
Expected / Actual / ขั้นสุดท้ายที่ผ่าน:
Error code ที่ล้างข้อมูลสำคัญแล้ว:
Root cause: ยืนยันแล้ว / สมมติฐาน / ยังไม่ทราบ
ข้อแก้ / ไฟล์ต้นทาง:
Regression command + exit code:
HOST / BUILD / PHONE / BOARD evidence:
NOT RUN และเหตุผล:
```

เก็บรหัสจริง, `[LOCAL_PAIR]`, Wi-Fi/Broker passwords, Flash backup และข้อมูลกุญแจไว้ในพื้นที่ local ที่เหมาะสม ไม่แนบกับแบบบันทึกนี้

## 5. เอกสารทางการที่ใช้ประกอบบทเรียน

- [Google Protobuf Lite และ R8](https://github.com/protocolbuffers/protobuf/blob/main/java/lite.md) และ [consumer rules ของ Espressif รุ่นที่ vendor](https://github.com/espressif/esp-idf-provisioning-android/blob/dfd3ce489b0881f0a8cedbccc9a9b1ea6f9b8a90/provisioning/consumer-rules.pro)
- [Android BluetoothGatt](https://developer.android.com/reference/android/bluetooth/BluetoothGatt) — API เริ่มงาน async และ callback
- [Espressif protocomm](https://docs.espressif.com/projects/esp-idf/en/v5.5.2/esp32s3/api-reference/provisioning/protocomm.html) — Security2 และ transport
- [Expo Continuous Native Generation](https://docs.expo.dev/workflow/continuous-native-generation) — เก็บข้อแก้ที่สร้าง native project ซ้ำได้
- [PAIRING_DIAGNOSIS](../PAIRING_DIAGNOSIS.md) — ลำดับการค้นสาเหตุและลิงก์ต้นทางเพิ่มเติม

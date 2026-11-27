# การตัดสินใจ — S3 HomeGuard

| ID | ข้อสรุป | เหตุผล / ผลต่อการตรวจรับ |
|---|---|---|
| D01 | สร้าง Lab09 แยก ไม่แก้ Lab07 | เก็บบท/แอปเดิมเป็น baseline ที่ย้อนเทียบได้ |
| D02 | ชื่อ S3 HomeGuard, package `com.esp32lab.homeguard`, ไอคอนบ้านในโล่สีเขียว | ติดตั้งคู่แอป Lab07 ได้; เป็นชื่อบทเรียน ไม่ใช่ใบรับรอง Level5 |
| D03 | ESP32-S3: Relay16/17, DHT4, OLED8/9, ปุ่ม18 | ใช้สายเดิมทั้งหมด OLED เป็น SH1106/SH1106G 128×64 ตามผู้สอน |
| D04 | Runtime ใช้ ESP-IDF APIs ที่รวมใน Arduino-ESP32 core3.3.13 โดยตรง | ตรวจ SDK แล้วเปิด Security2 patch1 ได้ ใช้ network_prov_mgr/ESP-MQTT/mbedTLS ไม่ใช้ WiFiProv Security1/PubSubClient แทน; ยังไม่เปิด production hardening |
| D05 | จับคู่ขั้นต่ำสอง transport: BLE และ Wi-Fi SoftAP ใช้ protocomm Security2 | มีรหัส SRP username/password เฉพาะเครื่องให้กรอกเองได้ QR เป็นเพียงทางเลือกกรอกข้อมูล; ปิด Security0 และห้ามลดระดับเงียบ ๆ |
| D06 | กดปุ่มบนอุปกรณ์เปิดหน้าต่าง 120s; OLED และ Serial ท้องถิ่นใช้ดูข้อมูลยืนยัน | รหัส SRP มี entropy สูง ไม่ใช้ PIN6หลักเป็น root key; Serial แสดงได้เฉพาะหน้าต่างจับคู่ที่ผู้สอนร้องขอ ไม่ส่งเข้า remote log/export |
| D07 | LAN/mDNS ค้นหาได้แต่ไม่ยืนยันตัวตน | เสนอเป็น phase เสริม ต้อง physical confirm + authenticated protocol ก่อนส่งคีย์; IP/manual entry เป็น fallback ของ discovery ไม่ใช่วิธีข้าม handshake |
| D08 | EMQX Public เป็นโปรไฟล์ทดลองฟรีผ่าน TLS8883/WSS8084; MQTTX เป็น client | แก้ความคลาดเคลื่อนจาก source ที่ต้อง private broker; public profile ไม่มี ACL gate จึงห้ามใช้แทน production acceptance |
| D09 | เปลี่ยน broker ผ่าน local authenticated pairing และ transaction stage→test→confirm→commit | Root key แยกจาก broker credential; ไม่มี auto fallback เป็น plaintext/กลับ public; ทั้งโทรศัพท์และ ESP ต้องยืนยัน endpoint ตรงกันก่อนสั่งงาน |
| D10 | Native Kotlin ถือ root/session key, MQTT และ crypto; JS ถือ opaque device handle | เก็บ key ที่ wrap ใน Android Keystore และปิด backup; ไม่ใช้ MQTT.js plaintext ของ Lab07 เป็น fallback |
| D11 | ส่งออกจากแอปได้เฉพาะ public bootstrap config | ไม่สร้าง `.ino` ที่ฝัง root/Wi-Fi password; upload ใช้ Arduino IDE/ESP-IDF บน PC ตามปกติ USB-OTG เข้ารหัสเป็นโจทย์เสริม ไม่มีการอ้างว่าแอป flash บอร์ดได้แล้ว |
| D12 | Dev board ไม่ burn eFuse; production hardening เป็น gate แยก | ไม่เปลี่ยน secure boot/flash encryption หรือบัญชี/billing ในงานสร้างบทเรียน |
| D13 | ตรวจ Release SRP packet/self-test ตั้งแต่ P0 เพิ่มจาก JVM | R8/Protobuf เคยทำให้ build ผ่านแต่จับคู่ไม่ได้; ปิดปัญหาที่ทำบนคอมพิวเตอร์/โทรศัพท์ได้ก่อนขอ physical window |
| D14 | แบ่งคู่มือตามงาน Build/Configuration/User/Known issues และมีสารบัญเดียว | ลดการปะปน starter/reference และให้แก้ค่าตั้ง/พฤติกรรม/ผลตรวจในไฟล์เจ้าของข้อมูล |
| D15 | ระบุ schema และค่าที่ source ใช้จริง พร้อมความต่างจาก acceptance | SQLite ปัจจุบันเป็น public cache/events; Telemetry QoS1/ESP keepalive30/filter20 ยังต่างเป้าหมายเดิม ไม่ลด HG เพื่อให้ผ่าน |
| D16 | เก็บ TEST_REPORT/PAIRING_DIAGNOSIS เป็นประวัติพร้อมสถานะล่าสุดด้านบน | ผลเก่าที่ NOT RUN ไม่ลบประวัติ และผล BLE/GPIO ไม่แทน SoftAP/migration/production |

คำว่า atomic ในเป้าหมาย broker migration ต้องตรวจ failure/recovery สองฝั่งเพิ่ม การเขียน config แบบ atomic ฝั่งเดียวไม่พิสูจน์ distributed atomicity เมื่อ ACK หาย/ไฟดับ ดู test-plan ก่อนอ้าง D09 ผ่านครบ

## ข้อปรับจาก Secure-IoT-Level-5.md

Security2 ใช้ SRP6a username/password, salt/verifier ตาม API ของ Espressif ส่วนคำว่า PoP ใน source ใช้ความหมายทั่วไปของการยืนยันว่าผู้เรียนอยู่ใกล้อุปกรณ์ ไม่ใช้ security1 PoP parameter ไปป้อน security2 โดยตรง

DHT11 ต้องอ่านห่างอย่างน้อย 2s ตามชุดคอร์ส ไม่ใช้ sampling100–500msที่ source เสนอรวมสำหรับ sensor ทั่วไป งานฉบับนี้ยังไม่อ้าง P0–P9 ผ่านครบ ไม่อ้าง hardware security level, performance p95 หรือการป้องกันการโจมตีจริงจาก unit tests

## การทดสอบกับบอร์ดเปล่า

ผู้สอนมี ESP32-S3 ที่ไม่มีเซ็นเซอร์ในรอบนี้ จึงเพิ่ม `HG_BARE_BOARD=true` ใช้ BOOT0 เป็น physical pairing/revoke และส่ง DHT invalid/null ไม่เปลี่ยนพิน OUT18 ของชุดโมดูลจริง ตรวจระดับ GPIO16/17 ผ่าน input buffer ได้ แต่ยังไม่ยืนยันหน้าสัมผัส Relay หรือค่าจากเซ็นเซอร์ บอร์ดเดิมสำรอง Flash16MB ในพื้นที่ local ที่ไม่ commit ก่อนอัปโหลด ไม่มีการแก้ eFuse

Runtime จอง BLE memory ผ่าน `esp32-hal-alloc-ble-mem.h` เช่นเดียวกับ WiFiProv ของ core3.3.13; ขอ stop แล้วรอ `ARDUINO_EVENT_PROV_END` โดยให้ WiFiGeneric ของ core เป็นผู้ deinit เพียงจุดเดียว ก่อนล้าง SRP และกลับเครือข่ายเดิม ตาราง partition คง app0 ที่0x10000ตาม Arduino uploader และวาง NVS256KB ที่0x610000เพื่อเก็บ committed/candidate config แบบ atomic ข้อมูล NVS ของ development ยังไม่ได้เข้ารหัส

ผลค้นสาเหตุ GATT/SDK/Activity lifecycle และการทดสอบที่จับบั๊กเดิมได้อยู่ใน [PAIRING_DIAGNOSIS](PAIRING_DIAGNOSIS.md) ระหว่าง Android ขออนุญาตเชื่อม Wi-Fi เก็บเฉพาะ network request ที่ยังไม่มี secure session; การแลกกุญแจเริ่มเมื่อ HomeGuard กลับ foreground และ MQTT control ถูกล็อกทันทีเมื่อ Activity pause

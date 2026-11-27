# ผลค้นสาเหตุการจับคู่ — 27 พฤศจิกายน 2026

เอกสารนี้เก็บลำดับการวิเคราะห์ตามรุ่น **ผลล่าสุดมี BLE pairing/Session/GPIO ผ่านแล้วใน 2.0.5** ส่วน SoftAP/migration ยังไม่ผ่านครบ ดู [สถานะล่าสุด](TEST_REPORT.md) และ [รายการปัญหาพร้อม regression](app-blueprint/KNOWN_ISSUES.md) ข้อความ NOT RUN ในช่วงต้นด้านล่างเป็นสถานะขณะวิเคราะห์นั้น

ปัญหาหลักอยู่ใน integration ที่เตรียมไว้ ไม่ได้เกิดจากผู้สอนกดยืนยันไม่ครบ ขณะวิเคราะห์นี้ยังไม่นับจับคู่ BLE/SoftAP end-to-end เป็น PASS

## จุดที่ยืนยันจากหลักฐาน

| จุด | หลักฐาน | ข้อแก้ |
|---|---|---|
| แอปค้างก่อน SRP handshake | Android log19:38:11 GATT connected status0; 19:38:12 MTU256 status0 และ service discovery status0 แล้วไม่มี descriptor callback | SDK ต้นทางเรียก readDescriptor() ภายใน Log.d() สองจุด การตัด log แบบเดิมทำให้คำสั่งอ่านหายไป คืนการอ่านแบบตรงและตรวจ return value |
| BLE controller เคย panic | backtrace ไป esp_bt_controller_init/queue delete; core3.3.13 มีการคืน BLE memory หากไม่มี library marker | include esp32-hal-alloc-ble-mem.h เหมือน WiFiProv รุ่นเดียวกัน; หลังแก้โทรศัพท์เชื่อม GATT/ค้นบริการได้แล้ว |
| ปิด provisioning ซ้ำ | เคย assert xQueueSemaphoreTake ตอนปิด; WiFiGeneric.cpp เรียก manager_deinit เองเมื่อรับ NETWORK_PROV_END | loop เรียก stop อย่างเดียว รอ ARDUINO_EVENT_PROV_END หลัง core ปิดเสร็จก่อนล้าง SRP/เปิด broker |
| อายุข้อมูล UUID | scheme_ble.h ระบุว่า API เก็บ pointer ก่อน start แล้วจึง copy | ย้าย UUID จากตัวแปรใน if block เป็น storage ตลอดอายุโปรแกรม |
| Activity pause ระหว่าง system dialog | Expo onHostPause ส่ง ACTIVITY_ENTERS_BACKGROUND; โค้ดเดิมปิด local ทุกครั้ง | รอเฉพาะ Wi-Fi system dialog ได้ แต่หยุด MQTT ทันที และไม่เริ่ม SRP/ส่งคีย์จน HomeGuard กลับ foreground |

MTU ที่วัดได้คือ256 จึงลด custom bundle chunk128→64bytes เพื่อเหลือพื้นที่สำหรับ JSON/base64/tag ข้อนี้ป้องกันปัญหาขั้นส่ง config ยังไม่ใช่หลักฐานว่ารอบที่ค้างได้ไปถึงขั้นนั้นแล้ว

## ตรวจซ้ำโดยไม่ต้องให้ผู้สอนกดปุ่ม

1. เพิ่ม BleDiscoveryTest ขับ GATT callback ด้วย mock: ต้องอ่าน descriptor ทุก endpoint แล้วจึงขอ proto-ver และรายงานผิดพลาดหากเริ่มอ่านไม่ได้
2. จำลองบั๊กเดิมเฉพาะ build cache โดยตัด read อีกครั้ง: 2 tests ล้มตามที่คาด (red) จากนั้นคืนโค้ดจริง
3. โค้ดแก้รัน native9/9ผ่าน: protocol7 + discovery2; assembleReleaseผ่าน ผล mock ไม่แทน hardware
4. ติดตั้ง APK2.0.3/code4 ผ่าน ADB แล้ว SHA256 `E20F948CE75FCB9FD7F354447A4A08B97B5EB48D2F1D5EAA63C2AF5C870757A7` และรัน crypto selfTest บน Samsung ผ่าน
5. Firmware compile/flash ตรวจ hash ผ่าน ใช้ Wi-Fi recovery หลัง local window ปิด เริ่ม GPIO16/17 เป็น OFF; การจับคู่/ปิดและเปิดซ้ำหลังแก้ล่าสุดยังต้องตรวจจริง

## ผลทดสอบแพ็กเกตแรกของ Security2 บน Android

หลังแก้การอ่าน descriptor รุ่น 2.0.3 เชื่อมต่อ GATT และแจ้งว่ากำลังยืนยันรหัส จากนั้นแอปแจ้ง `SECURE_OPERATION_FAILED` บอร์ดยังรายงาน `paired=0` จึงแยกทดสอบการสร้างข้อความแรกของ SRP ใน APK แบบ release ด้วยชื่อและรหัสทดสอบสาธารณะ ขั้นนี้ทดสอบได้โดยไม่เปิดบอร์ด

| การทดสอบ | Samsung S25 Ultra | Android emulator |
|---|---|---|
| APK 2.0.4 สร้างแพ็กเกตด้วยกฎ R8 เดิม | `SRP_PROTOBUF_SELFTEST_FAILED` | ผิดพลาดแบบเดียวกัน |
| APK 2.0.5 คงฟิลด์ของข้อความ Protobuf Lite ของ Espressif สร้างและอ่านแพ็กเกตกลับ | PASS | PASS |

ตัวสร้างแพ็กเกตใช้ชื่อฟิลด์ตอนรัน การนำ SDK เข้ามาในโปรเจกต์โดยขาดกฎสำหรับ R8 ทำให้รูปแบบ release ใช้ชื่อฟิลด์เหล่านั้นไม่ได้ เพิ่ม consumer rule เฉพาะคลาสข้อความของ Espressif และตรวจ packet จริงในเมนู **ความปลอดภัย → ทดสอบ crypto ในโทรศัพท์** แอปจะหยุดและแสดงขั้นที่ผิดพลาดเมื่อสร้างแพ็กเกต ตรวจ challenge หรือตรวจ proof ไม่สำเร็จ [เอกสาร Protobuf Lite ของ Google](https://github.com/protocolbuffers/protobuf/blob/main/java/lite.md) · [กฎของ Espressif SDK รุ่นที่ใช้](https://github.com/espressif/esp-idf-provisioning-android/blob/dfd3ce489b0881f0a8cedbccc9a9b1ea6f9b8a90/provisioning/consumer-rules.pro)

การขึ้น PASS ในเมนูนี้ยืนยันเฉพาะงานบนโทรศัพท์ การจับคู่จริงต้องรอให้โทรศัพท์ตรวจ proof จากบอร์ด และบอร์ดรายงาน `paired=1`

**ผลบนอุปกรณ์จริง** ใช้ Samsung S25 Ultra กับ ESP32-S3 บอร์ดเปล่า กรอกข้อมูลประจำอุปกรณ์และ Wi-Fi ไว้ก่อน กด BOOT ค้าง 2–3 วินาที ใช้รหัส Security2 ของรอบนั้นจาก USB Serial และยืนยันตัวตนบนโทรศัพท์ แอป 2.0.5 แสดง “จับคู่สำเร็จ” พร้อมอุปกรณ์ในหน้าหลัก บอร์ดบันทึกค่า ตอบ proof ปิดหน้าต่างจับคู่ และรายงาน `paired=1 wifi=1 mqtt=1` หลังเชื่อมกับ EMQX ผ่าน TLS เมื่อมีอุปกรณ์ในหน้าหลักแล้ว ใช้ปุ่ม **ยืนยันตัวตน / เปิดเซสชัน** เพื่อเริ่มควบคุมได้โดยตรง

เมื่อกดเปิดเซสชันและยืนยันบน Samsung แล้ว โทรศัพท์ขึ้น “ยืนยันแล้ว” และบอร์ดอ่านได้ `authenticated=1` ทดสอบปุ่มเปิดและปิด GPIO16 และ GPIO17 ครบทั้งสองช่อง ผลสุดท้ายแอปแสดง OFF ทั้งคู่และพินทั้งสองอ่านได้ HIGH ตามการต่อแบบ active LOW [ผลวัดแต่ละคำสั่ง](TEST_REPORT.md)

UI รุ่น 2.1.3 เปลี่ยนจากปุ่มเปิดเซสชันแยกเป็นการแตะการ์ดรีเลย์ที่ขึ้น **แตะเพื่อยืนยัน** รอ State แล้วแตะครั้งถัดไปเพื่อสั่งงาน ปุ่มจับคู่อยู่ท้ายฟอร์ม ไม่ใช่ footer ของรุ่น 2.0.1 ดู [คู่มือปัจจุบัน](app-blueprint/USER_GUIDE.md)

## สิ่งที่ไม่ควรเปลี่ยนเพื่อแก้รอบนี้

- ยังไม่เปลี่ยน broker หรือรหัส Wi-Fi: ปัญหาค้างก่อน MQTT และเคยตรวจ Wi-Fi บนบอร์ด/TLS+MQTT บนโทรศัพท์ผ่านแล้ว
- ไม่ลด Security2 เป็น Security0/1 และไม่ปิด TLS verification
- แอป targetSDK36: Android17 ระบุว่าไม่ต้องขอ ACCESS_LOCAL_NETWORK จนกว่าจะ target37 จึงไม่เพิ่ม permission นี้โดยเดา

## เมื่อต้องจับคู่บอร์ดเครื่องใหม่

ผู้ช่วยติดตั้ง/กรอกข้อมูล/ดู Serial ให้ได้เอง ให้คงสาย USB โทรศัพท์และบอร์ดไว้ เปิด HomeGuard ค้าง เมื่อรุ่นแก้พร้อมให้กด **BOOT ค้าง2–3วินาทีแล้วปล่อย** และยืนยันลายนิ้วมือ/รหัสล็อกหน้าจอเมื่อระบบถาม **RESET/EN มีหน้าที่รีบูต** จึงไม่ใช้เปิดหน้าต่างจับคู่ และไม่กดระหว่างจับคู่ รหัสล็อกหน้าจอไม่ต้องบอกผู้ช่วย

## แหล่งทางการที่ตรวจ

- [Espressif Android SDK รุ่นที่ vendor](https://github.com/espressif/esp-idf-provisioning-android/blob/dfd3ce489b0881f0a8cedbccc9a9b1ea6f9b8a90/provisioning/src/main/java/com/espressif/provisioning/transport/BLETransport.java): readDescriptor ใน Log สองจุด
- [Android BluetoothGatt.readDescriptor](https://developer.android.com/reference/android/bluetooth/BluetoothGatt#readDescriptor(android.bluetooth.BluetoothGattDescriptor)): การเริ่มอ่านและ callback
- [Arduino core3.3.13 WiFiGeneric](https://github.com/espressif/arduino-esp32/blob/3.3.13/libraries/WiFi/src/WiFiGeneric.cpp): ปิด manager เมื่อรับ END
- [ESP-IDF protocomm](https://docs.espressif.com/projects/esp-idf/en/v5.5.2/esp32s3/api-reference/provisioning/protocomm.html): Security2 SRP6a + AES-GCM/สอง transport
- [Android Wi-Fi peer-to-peer request](https://developer.android.com/develop/connectivity/wifi/wifi-bootstrap): system consent/NetworkCallback/ปล่อย network หลังจบ
- [Expo SDK58 AppContext](https://github.com/expo/expo/blob/sdk-58/packages/expo-modules-core/android/src/main/java/expo/modules/kotlin/AppContext.kt): pause→background event
- [Android local network permission](https://developer.android.com/privacy-and-security/local-network-permission): target36 เทียบ37

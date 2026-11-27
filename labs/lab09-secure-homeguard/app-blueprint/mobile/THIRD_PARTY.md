# ซอฟต์แวร์จากภายนอก

| ส่วน | ต้นทาง / รุ่น | License / ข้อปรับ |
|---|---|---|
| Espressif Android provisioning core | [esp-idf-provisioning-android](https://github.com/espressif/esp-idf-provisioning-android/tree/dfd3ce489b0881f0a8cedbccc9a9b1ea6f9b8a90), commit `dfd3ce489b0881f0a8cedbccc9a9b1ea6f9b8a90` | Apache2.0; LICENSE-ESPRESSIF อยู่ใน module Android |
| HiveMQ MQTT client | `com.hivemq:hivemq-mqtt-client-shaded:1.4.2` | Apache2.0; ใช้ TLS/hostname validation ของ Android |
| Expo / React Native / React | รุ่นล็อกใน package-lock.json | ดู license ของแต่ละ dependency ใน node_modules หลัง npm ci |
| EMQX public CA | [เอกสาร public broker](https://www.emqx.com/en/mqtt/public-mqtt5-broker) | CA เป็นข้อมูลสาธารณะ ใช้ตรวจเซิร์ฟเวอร์ ไม่ใช่ application key |

SDK ที่ vendor อยู่ใน `modules/homeguard-security/android/vendor/` เก็บ Java/proto ส่วนที่ใช้จริง ตัดกล้อง/QR/SDK manager และ implementation Security0/1 ออกจาก build; protocol definitions เดิมอาจยังมี enum ของรุ่นเก่าเพื่อ compatibility แต่ bridge ยอมรับ Security2 patch1 เท่านั้น

ข้อแก้จากต้นทาง: ลบ credential/payload logs และ stack traces; ตรวจ security version/patch ก่อนเริ่ม; ห้ามสร้าง security fallback; ใช้ GCMParameterSpec128/ตรวจ counter overflow; destroy session และล้าง buffer; ห้ามส่ง plaintext เมื่อ session ไม่ established; SoftAP มี timeout/response limit และไม่ตาม redirect

แอปเก็บ source ของ native module แยกจาก Android ที่ Expo prebuild สร้าง ห้ามย้ายข้อแก้เหล่านี้ไปอยู่เฉพาะ generated `android/`

การตรวจ BLE บนเครื่องจริงพบว่า upstream เรียก readDescriptor() อยู่ภายใน Log.d() สองจุด การตัด log ต้องคง side effect นี้ไว้: vendor เรียก readDescriptor แบบตรงและตรวจ return value แล้ว ใช้ chunk64bytes เพื่อให้ custom endpoint อยู่ใน MTU256 ที่ทดสอบบน ESP32-S3

Release integration ต้องคง `consumer-rules.pro` (องค์ประกอบที่ผู้เรียนพัฒนาเอง; source อ้างอิงเก็บสำหรับผู้สอน) สำหรับ Protobuf Lite/Espressif และตรวจ SRP first-packet self-test บน Android Release เพิ่มจาก JVM tests เคยพบปัญหานี้ใน 2.0.4 และแก้ใน 2.0.5 ตาม [KNOWN_ISSUES I-02](../KNOWN_ISSUES.md) เมื่ออัปเดต vendor/Protobuf/R8 ให้ตรวจ license/provenance และ regression ใหม่ ไม่คัดลอก consumer rules โดยไม่ตรวจรุ่นที่ใช้

# ความคืบหน้า Lab09 — สถานะล่าสุด

> ฉบับสาธารณะ: เอกสารอธิบายแบบและผลอ้างอิงสำหรับผู้เรียนสร้างเอง source แอป/secure runtime สำเร็จเก็บแยกสำหรับผู้สอน ไม่ได้อยู่ใน repository นี้

อัปเดต 27 พฤศจิกายน 2569 · [เริ่มใช้งาน/พัฒนา](app-blueprint/README.md) · [ผลจริงและประวัติแต่ละรุ่น](TEST_REPORT.md)

## สิ่งที่มีอยู่แล้ว

| ส่วน | สถานะ / หลักฐาน |
|---|---|
| บทเรียนและ starter | README/ภาพ/คำถาม/ZIP preflight3ชุด/helperLab07 มีแล้ว; starter ยังไม่ใช่ native runtime |
| แอปอ้างอิง | React Native/Expo + Native Kotlin/local module/lockfile/build script และ Release APK มีแล้ว รุ่น UI ล่าสุดที่บันทึกคือ2.1.3 |
| Development firmware | Arduino3.3.13 เรียก ESP-IDF Security2/ESP-MQTT/mbedTLS คงพินS3 และ bare-board mode |
| จับคู่ BLE | Samsung S25 Ultra + ESP32-S3 บอร์ดเปล่า จับคู่/proof/บันทึกเจ้าของสำเร็จตามรายงาน2.0.5 |
| Application Session / คำสั่ง | เปิดSessionผ่านEMQXTLS และสั่งGPIO16/17 ON/OFF อ่านระดับขาแล้ว; ไม่มีโมดูลRelayจริง |
| UI/เอกสาร | ห้าแท็บCompact Center, แตะการ์ดเพื่อยืนยัน, ปุ่มจับคู่ท้ายฟอร์ม; มี SYSTEM_WORKFLOW และคู่มือแยกตามงาน |

## งานที่ยังต้องทำเพิ่ม

- SoftAP Security2 บน Android+บอร์ดครบ workflow รวม consent/deny/cancel และ cleanup รอบถัดไป
- Broker migration/recovery สองฝั่ง และ wrongCA/ACKหาย/ไฟดับ/reboot/timeout
- Replay/Lease/reboot/rate/queue/flood/1000 commands/24h soak บน hardware ตามแผน ไม่ยืมผล unit tests
- โมดูล DHT11/OLED/ปุ่ม18/หน้าสัมผัส Relay และ feedback/โหลดจริง
- Private ACL, server-side attempts/cooldown, root rotation/revoke/recovery, encrypted NVS/Flash, Secure Boot/Signed OTA/independent review
- ตัดสินความต่างจาก requirement: Telemetry QoS1/ESPkeepalive30/filter20packets/s และ schema/migration ตามคู่มือ

## วิธีเริ่มรอบพัฒนาถัดไป

1. ใช้ BUILD_GUIDE และ Release self-test ให้ผ่านก่อนเปิด physical pairing
2. เลือก issue/gate ที่จะทำจาก KNOWN_ISSUES/test-plan แล้วทำทีละชั้น
3. เก็บ commit+working-tree status, APK/firmware/hash/รุ่นอุปกรณ์ และผลแยก HOST/BUILD/PHONE/BOARD/NOT RUN
4. อัปเดตค่าตั้ง/contract/user guide/DECISIONS และรายงานเมื่อพฤติกรรมเปลี่ยน

Checklist P0–P9 ใน app-blueprint เป็นแม่แบบของผู้เรียน จึงคงว่าง ไม่ใช่รายการยืนยันว่าแอปอ้างอิงยังไม่มี Native หรือไม่เคยจับคู่ ส่วนผลชุดเริ่มต้นในช่วงต้น TEST_REPORT เป็นประวัติ ไม่ใช่สถานะล่าสุดทั้งหมด

# ดาวน์โหลดแอปตัวอย่างสำหรับทดลอง

[ดาวน์โหลด Lab07 Basic APK 1.0.1](https://github.com/sunanedu/ESP32-LAB/raw/refs/heads/main/downloads/esp32-smart-home-preview-1.0.1.apk)

- ไฟล์ `esp32-smart-home-preview-1.0.1.apk` ขนาด 43,164,520 bytes
- Standalone Android APK รองรับ arm64-v8a/x86_64 ไม่ต้องเปิด Metro
- เป็นแอป Smart Home พื้นฐานผ่าน public MQTT broker สำหรับทดลองในห้องเรียน ไม่ใช่ Lab09 รุ่น Security2/AEAD
- ใช้ test signing key สำหรับแล็บ ไม่ใช่การเผยแพร่ผ่าน Play Store
- นักเรียนใช้ APK ดูหน้าตาและเทียบพฤติกรรม แล้วสร้างแอปของตนจาก Markdown/ใบงาน ไม่มีซอร์สแอปสำเร็จแจกใน repository นี้

SHA-256:

```text
433f0b805f2f1f5967518c41d46f4e8ae9416f7f58468501d57614c03d858cf7
```

หลังดาวน์โหลดบน Windows ตรวจด้วย `Get-FileHash -Algorithm SHA256 <พาธไฟล์ APK>` แล้วเทียบค่าข้างบน

[เริ่มสร้างแอป Lab07](../labs/lab07-smart-home-project/app-blueprint/README.md) · [ต่อยอด Lab09](../labs/lab09-secure-homeguard/app-blueprint/README.md) · [สารบัญคอร์ส](../README.md)

APK เดิมเคยตรวจบน Samsung กับ ESP32-S3 บอร์ดเปล่าตามรายงาน ไม่ยืนยันเซ็นเซอร์/หน้าสัมผัสรีเลย์จริง การส่งงานต้องใช้ source และผลทดสอบที่ผู้เรียนสร้างเอง

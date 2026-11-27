# นโยบายการเผยแพร่บทเรียนและแอป

อัปเดต 27 พฤศจิกายน 2569

- Repository สาธารณะเผยแพร่บทเรียน Markdown วงจร ภาพ แบบฝึก สเปกสำหรับสร้างแอป และโค้ด ESP32 ที่ใช้สอน
- ไม่เผยแพร่ implementation แอปอ้างอิงสำเร็จของ Lab07/Lab09, Native security module, helper ที่คัดลอกแอปสำเร็จ หรือ secure development firmware ของผู้สอน
- สำหรับทดลองใช้งาน แจกเฉพาะ [APK พื้นฐาน Lab07 1.0.1](downloads/README.md) ไม่แจก APK รุ่น Security2/AEAD ของ Lab09
- ไม่ใช้ GitHub Releases หรือแท็กแจกซอร์ส นักเรียนดาวน์โหลด APK โดยตรงจาก downloads/ แล้วสร้างแอปของตนจาก app-blueprint/ และใบงาน
- source/ประวัติ/รีลีสเดิมทั้งหมดเก็บแยกสำหรับผู้สอน ไม่เป็น dependency ที่ผู้เรียนต้องเข้าถึง

ก่อนเผยแพร่ต้องรัน `python scripts/validate-public-app-policy.py`, `python scripts/validate-pinout.py` และ `pwsh -File scripts/validate-repository.ps1` ห้ามนำ private working tree หรือประวัติเดิมมารวมด้วยการ mirror/force-push ลงชุดสาธารณะ

การคง APK เดิมไม่รับประกันว่าผู้ดาวน์โหลดจะวิเคราะห์ไฟล์ไม่ได้ จึงห้ามฝังรหัสจริงลง APK เช่นเดียวกับเดิม

# Lab07 — ภาพคาดการณ์แอป React Native 4 หน้า

ภาพประกอบสร้างด้วย AI โดยใช้เครื่องมือ imagegen ในตัว ตามสเปก app-blueprint/requirements.md และ design.md ไม่ใช่ screenshot หรือหลักฐานแอปที่รันจริง

ไฟล์ปลายทาง: `labs/lab07-smart-home-project/app-blueprint/assets/lab07-app-four-screens-ai.png`

## Prompt

```text
Use case: ui-mockup
Asset type: one high-resolution landscape contact sheet for a Thai ESP32 React Native Android mini-project lesson.
Primary request: Generate ONE image containing EXACTLY FOUR different complete portrait mobile application screens, arranged evenly left to right in one straight row. The four screens are Dashboard, Add Device, Edit Device, Settings. These are projected UI designs from a Markdown specification, not photos of a finished app.
Visual style: premium minimalist smart-home interface, brilliant white #FFFFFF page and cards, crisp very light gray #E7EBF0 1px card borders, extremely subtle short soft shadows, 20px rounded cards and 14px inputs/buttons, generous consistent whitespace, dark navy #17212F typography, muted #64748B captions, restrained blue #2563EB primary buttons and pale blue icon circles. Clean attractive Apple Home / Google / Xiaomi-level simplicity, but original Android app, no company logos or copied branding. Use consistent elegant Ionicons-style icon-font glyphs (outlined home, settings gear, plus, back arrow, thermometer, water drop, lightbulb, plug, power, edit pencil, database). No emoji. No dark mode, no gradients, no glass, no heavy shadows, no photographic decor. Phone screenshots on a white page with slim rounded light-gray boundaries, not thick black device mockups; all four screen interiors fully visible, readable and unclipped. Large resolution, enough space for each screen; do not overlap or angle devices.
UI text primarily Thai, render legibly and spell as specified. Technical values remain exact. Same profile across screens: device name "ชุดทดลอง 01", room "ห้องเรียน", STUDENT_ID "desk-a7f2", prefix "home/desk-a7f2".
SCREEN 1 — Dashboard:
Header "Smart Home", smaller "บ้านของฉัน", plus and gear icon buttons at right.
Selected device compact white card: home icon, "ชุดทดลอง 01", "ห้องเรียน", edit pencil.
Two separate small status pills "Broker เชื่อมต่อแล้ว" and "ESP32 ออนไลน์".
Two equal sensor cards in a row: thermometer "อุณหภูมิ" with large "28.0 °C"; droplet "ความชื้น" with large "61 %RH"; small "อัปเดตล่าสุด 2 วินาที" / valid indicator.
Section "ควบคุมอุปกรณ์", subtle secondary action "ปิดทั้งหมด".
Two white relay cards: bulb icon "ไฟสาธิต", caption "Relay 1", reported state "เปิด", blue circular power button; plug icon "โหลดสำรอง", "Relay 2", state "ปิด", neutral outlined power button. Clear report state, no arbitrary schedules.
Small "ประวัติล่าสุด" card with two brief local events and "ดูประวัติ". Footer database icon "เก็บข้อมูลในโทรศัพท์".
SCREEN 2 — Add:
Back arrow, title "เพิ่มอุปกรณ์". Form fields with labels and example entries:
"ชื่ออุปกรณ์" / "ชุดทดลอง 01"; "ห้อง / พื้นที่" / "ห้องเรียน"; "STUDENT_ID" / "desk-a7f2".
Small helper "ต้องตรงกับ firmware ESP32".
Label "ไอคอนอุปกรณ์" and four outlined icon choices, first selected in pale blue.
Read-only pale surface preview "MQTT prefix" and "home/desk-a7f2".
Blue full-width button "บันทึกอุปกรณ์"; secondary "ยกเลิก".
SCREEN 3 — Edit:
Back arrow, title "แก้ไขอุปกรณ์". Same device name and room editable fields, same four icon choices. Locked muted field "STUDENT_ID" value "desk-a7f2" and prefix "home/desk-a7f2"; small lock icon.
Two more editable fields "ชื่อ Relay 1" / "ไฟสาธิต", "ชื่อ Relay 2" / "โหลดสำรอง".
Full-width blue "บันทึกการแก้ไข"; secondary "ยกเลิก"; restrained red text action "ลบอุปกรณ์จากโทรศัพท์". Clearly different screen from Add, no invented extra sensor types.
SCREEN 4 — Settings:
Back arrow, title "ตั้งค่า". White card section "การเชื่อมต่อ MQTT".
Protocol read-only "WSS"; host field "broker.emqx.io"; port "8084"; path "/mqtt". Compact client ID read-only "rn_a7f2..." with copy icon.
Switch "เชื่อมต่ออัตโนมัติ" enabled. Small connection status "เชื่อมต่อแล้ว".
Blue button "บันทึกและเชื่อมต่อ".
Section "ข้อมูลในโทรศัพท์" with database icon, "SQLite", "เก็บประวัติในเครื่อง"; secondary button "ล้างประวัติ".
Small footer "Public broker • ไม่ส่งข้อมูลส่วนตัว".
Overall: faithful realistic implementable React Native app UI, excellent alignment and readable typography. Place small unobtrusive numbered labels above each screen: "01 Dashboard", "02 Add Device", "03 Edit Device", "04 Settings". No marketing headlines, no splash/login screens, no charts covering the controls, exactly four screens, no fifth screen.
```

"""ภาพ Lab08 สร้างเองจาก SVG: python scripts/generate-lab08-images.py --render-png."""
import argparse
from pathlib import Path
import runpy
import subprocess
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
LAB = "lab08-hardware-check"
OUT = ROOT / "labs" / LAB / "images"
SHARED = runpy.run_path(str(ROOT / "scripts/generate-wiring-diagrams.py"))
Diagram = SHARED["Diagram"]
ORANGE, BLACK, BLUE = (SHARED[k] for k in ("ORANGE", "BLACK", "BLUE"))


def adapted(source, name, replacements):
    content = (ROOT / "labs" / source).read_text(encoding="utf-8")
    for old, new in replacements:
        content = content.replace(old, new)
    (OUT / (name + ".svg")).write_text(content, encoding="utf-8")


def generate():
    OUT.mkdir(parents=True, exist_ok=True)
    adapted("lab05-relay-ultrasonic/images/lab05-ultrasonic-wiring.svg", "lab08-ultrasonic-wiring", [
        ("LAB 05", "LAB 08.2"),
        ("5.2 ultrasonic_read / 5.3 filter ใช้ชุดนี้; 5.4 distance_relay ใช้ร่วมกับ Relay16/17", "ทดสอบระยะทีละโมดูล: เทียบไม้บรรทัด 10/20/50 cm และจดกรณี TIMEOUT แยกจากระยะ")])
    adapted("lab05-relay-ultrasonic/images/lab05-relay-wiring.svg", "lab08-relay-wiring", [
        ("LAB 05", "LAB 08.4"),
        ("5.1 relay_basic ใช้ชุด Relay; 5.4 distance_relay เพิ่ม HC-SR04 จากภาพก่อนหน้า", "ทดสอบ OFF/OFF → ON/OFF → OFF/ON → ON/ON ทุก 2 วินาที แล้ววนกลับ")])
    adapted("lab04-lcd-i2c/images/lab04-traffic-light-wiring.svg", "lab08-traffic-light-wiring", [
        ("LAB 04", "LAB 08.3"),
        ("4.4 traffic test: common VCC", "8.3 light test: common VCC")])
    adapted("lab04-lcd-i2c/images/lab04-button-module-wiring.svg", "lab08-button-wiring", [
        ("LAB 04", "LAB 08.5"), ("4.3 button test", "8.5 button test")])
    adapted("lab04-lcd-i2c/images/lab04-oled-wiring.svg", "lab08-oled-wiring", [
        ("LAB 04", "LAB 08.6"),
        ("4.1 Scanner / 4.2 OLED test: ต่อ 4 สายนี้ • SDA คือข้อมูล / SCL คือ clock", "8.6 OLED test: แสดงข้อความแรก 3 วินาที แล้วแสดงชื่อ/uptime/แถบ DEMO")])
    d = Diagram("LAB 08.1 | ESP32-S3 → DHT11", "ตรวจอุณหภูมิและความชื้น: DATA4 / VCC3V3 / GND • เลือกโมดูลรองรับ 3.3V")
    d.board(y=230, h=640)
    d.card(1095, 230, 435, 640, "DHT11 โมดูล 3 พิน", "อ่านชื่อพินจริง VCC/DATA/GND", "#eff7ff")
    d.wire(420, "3V3", "VCC / +", color=ORANGE)
    d.wire(580, "GPIO4", "DATA / S / OUT", color=BLUE)
    d.wire(760, "GND", "GND / −", color=BLACK)
    d.notes(["ถ้าไม่มี pull-up ในตัว: ต่อ R10kΩ ระหว่าง DATA กับ 3V3 ไม่ต่อ R อนุกรมกับ DATA",
             "แบบเปล่า 4 ขา: มองหน้าตะแกรง ขาชี้ลง 1 VCC / 2 DATA / 3 NC ไม่ต่อ / 4 GND",
             "อ่านทุก 2 วินาที; READ FAILED คืออ่านไม่ได้ ไม่ใช่อุณหภูมิ 0°C; ห้ามดึง DATA ขึ้น 5V"])
    d.save(LAB, "lab08-dht11-wiring")

    d = Diagram("LAB 08 | ชุดตรวจเช็กอุปกรณ์ 6 โมดูล", "ทดสอบทีละชุดและบันทึกผลจริงก่อนนำไปทำแล็บอื่น • พิน ESP32-S3 คงเดิม")
    cards = [(65,225,"DHT11","DATA4 / 3V3 / GND", "อุณหภูมิ + ความชื้น"),
             (575,225,"HC-SR04","TRIG6 / ECHO7 divider", "ระยะทาง + timeout"),
             (1085,225,"ไฟ RD / YL / GY","10 / 11 / 12 / VCC3V3", "ตรวจสีทีละช่อง"),
             (65,590,"Relay 2CH","IN1=16 / IN2=17 / 5V", "ตรวจเปิด–ปิดสองช่อง"),
             (575,590,"ปุ่มโมดูล 3 พิน","OUT18 / VCC3V3 / GND", "ตรวจ raw + การกดค้าง"),
             (1085,590,"OLED I2C","SDA8 / SCL9 / 3V3 / GND", "ข้อความ + UI เปลี่ยนค่า")]
    for x,y,title,pins,goal in cards:
        d.card(x,y,450,310,title,pins)
        d.rect(x+35,y+110,380,115,"#e4eff9","#7498b8",7)
        d.text(x+225,y+158,title,27,anchor="middle")
        d.text(x+225,y+200,"MODULE",21,anchor="middle")
        d.text(x+225,y+275,goal,24,anchor="middle")
    d.notes(["ภาพอุปกรณ์เป็นสัญลักษณ์เชิงอธิบาย ไม่อ้างรูปร่าง/ตำแหน่ง header ของทุกรุ่น",
             "ทุกโมดูลมีภาพพินต่อพินแยกในบทเรียน; code ZIP เปิดทีละโฟลเดอร์ใน Arduino IDE",
             "ผล compile/ภาพจำลองไม่แทนผลวัดอุปกรณ์จริง ให้ผู้เรียนบันทึก ผ่าน/ตรวจเพิ่ม ตามใบงาน"])
    d.save(LAB, "lab08-equipment-overview")

    d = Diagram("LAB 08.6 | ตัวอย่าง UI ของ OLED", "ภาพจำลอง layout จากโค้ด ไม่ใช่ภาพถ่ายผลทดสอบ • ค่า uptime มาจากเวลา MCU ไม่ใช่อุณหภูมิ")
    d.card(65,230,600,630,"ข้อความเริ่มต้น","แสดงก่อนเข้าสู่ UI เป็นเวลา 3 วินาที")
    d.rect(95,360,540,290,"#101b27","#23344a",5)
    for y,text in [(405,"OLED READY"),(470,"SSD1306 128x64"),(535,"Address: 0x3C"),(600,"UI starts in 3 s")]:
        d.text(120,y,text,30,"#c6f1ff")
    d.card(700,230,835,630,"UI หลังครบ 3 วินาที","ชื่อ / uptime เปลี่ยนทุกวินาที / แถบ DEMO")
    d.add('<g transform="translate(730 350) scale(6)">')
    d.add('<rect x="0" y="0" width="128" height="64" fill="#101b27" stroke="#c6f1ff" stroke-width="0.3"/>')
    d.text(4,11,"DEVICE CHECK",7,"#c6f1ff")
    d.path([(1,14),(126,14)],"#c6f1ff",0.5)
    d.text(4,25,"Name: S3_OLED",7,"#c6f1ff")
    d.text(4,45,"42 s",14,"#c6f1ff")
    d.text(4,56,"RUN  DEMO 42%",7,"#c6f1ff")
    d.add('<rect x="4" y="59" width="120" height="3" fill="#101b27" stroke="#c6f1ff" stroke-width="0.2"/>')
    d.add('<rect x="5" y="60" width="49" height="1" fill="#c6f1ff"/>')
    d.add('</g>')
    d.text(735,795,"แถบ DEMO วน 0–100% เพื่อสังเกตการวาดใหม่",23)
    d.notes(["หัวข้อและชื่ออยู่ตำแหน่งคงที่; ค่าและแถบเปลี่ยนโดยใช้ millis() กำหนดรอบ",
             "clearDisplay() ล้าง buffer → วาดชื่อ/ค่าใหม่ → display() ส่งภาพไปจอ",
             "ตัวอย่างใช้ font ASCII; ให้โจทย์จัดหน้า UI ใหม่แก่ผู้เรียนโดยไม่แจกโค้ดโจทย์ต่อยอด"])
    d.save(LAB, "lab08-oled-ui-result")
    (OUT/"download-sketch.svg").write_text('''<svg xmlns="http://www.w3.org/2000/svg" width="360" height="64" viewBox="0 0 360 64" role="img" aria-label="ดาวน์โหลดสเก็ตช์ ZIP"><rect x="1" y="1" width="358" height="62" rx="9" fill="#1567bf"/><g fill="white" font-family="Tahoma,Segoe UI,sans-serif"><path d="M24 17h8v18l7-7 5 5-16 15-16-15 5-5 7 7z"/><text x="62" y="41" font-size="23">ดาวน์โหลดสเก็ตช์ (.zip)</text></g></svg>''', encoding="utf-8")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--render-png", action="store_true")
    parser.add_argument("--browser", type=Path, default=Path("C:/Program Files/Google/Chrome/Application/chrome.exe"))
    args = parser.parse_args()
    generate()
    if args.render_png:
        if not args.browser.is_file():
            parser.error("ระบุ --browser เป็น path ของ Chrome/Chromium")
        profile = ROOT / "build/lab08-qa/chrome"
        profile.mkdir(parents=True, exist_ok=True)
        for svg in sorted(OUT.glob("lab08-*.svg")):
            attr = ET.parse(svg).getroot().attrib
            png = svg.with_suffix(".png")
            subprocess.run([
                str(args.browser), "--headless", "--disable-gpu", "--hide-scrollbars",
                "--allow-file-access-from-files", "--disable-background-networking", "--no-first-run",
                "--virtual-time-budget=1000", f"--user-data-dir={profile}",
                f"--screenshot={png}", f"--window-size={attr['width']},{attr['height']}", svg.as_uri(),
            ], check=True, capture_output=True, timeout=45)
            if not png.is_file():
                raise RuntimeError(f"ไม่ได้สร้าง {png}")
            print(png.relative_to(ROOT))

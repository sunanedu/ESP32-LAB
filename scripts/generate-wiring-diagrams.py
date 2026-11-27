"""สร้าง SVG ต่อสาย Lab02–06; ใช้ชื่อพินเชิงหน้าที่ ไม่จำลองตำแหน่ง header.

รันจาก repository: python scripts/generate-wiring-diagrams.py
เพิ่ม --render-png --browser "C:/Program Files/Google/Chrome/Application/chrome.exe"
เพื่อ render PNG จาก SVG เดียวกันด้วย Chrome headless (ไม่ต้องติดตั้ง Python package).
"""

import argparse
from html import escape
from pathlib import Path
import subprocess
import runpy
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
W, H = 1600, 1150
INK, MUTED = "#17283f", "#52647a"
RED, ORANGE, BLACK = "#d83a42", "#d76a12", "#293544"
BLUE, TEAL, PURPLE = "#1769cb", "#008575", "#8555b5"


class Diagram:
    def __init__(self, title, subtitle, height=H):
        self.height = height
        self.parts = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{W}" height="{height}" viewBox="0 0 {W} {height}" role="img">',
                      f'<title>{escape(title)}</title><desc>{escape(subtitle)}</desc>',
                      f'<rect width="1600" height="{height}" fill="#fff"/>',
                      '<g font-family="Tahoma,Segoe UI,sans-serif">']
        self.rect(40, 30, 1520, 132, "#eef5fb", "none", 18)
        self.text(75, 87, title, 36, weight="bold")
        self.text(75, 132, subtitle, 23, MUTED)
        self.text(65, height - 32, "อ่านชื่อ GPIO/สกรีนบนอุปกรณ์จริง • ภาพเป็นผังการเชื่อมต่อ ไม่ใช่ตำแหน่งขาบน header", 23, MUTED)

    def add(self, value):
        self.parts.append(value)

    def text(self, x, y, value, size=25, color=INK, anchor="start", weight="normal"):
        self.add(f'<text x="{x}" y="{y}" font-size="{size}" fill="{color}" text-anchor="{anchor}" font-weight="{weight}">{escape(value)}</text>')

    def rect(self, x, y, w, h, fill="#fff", stroke="#bccbdb", radius=12):
        self.add(f'<rect x="{x}" y="{y}" width="{w}" height="{h}" rx="{radius}" fill="{fill}" stroke="{stroke}" stroke-width="2"/>')

    def path(self, points, color=BLUE, width=5, dashed=False):
        pts = " ".join(f'{x},{y}' for x, y in points)
        dash = ' stroke-dasharray="9 7"' if dashed else ''
        self.add(f'<polyline points="{pts}" fill="none" stroke="{color}" stroke-width="{width}" stroke-linejoin="round" stroke-linecap="round"{dash}/>')

    def dot(self, x, y, color=BLUE, r=6):
        self.add(f'<circle cx="{x}" cy="{y}" r="{r}" fill="{color}"/>')

    def card(self, x, y, w, h, title, subtitle="", fill="#f6f9fc"):
        self.rect(x, y, w, h, fill)
        self.text(x + 22, y + 42, title, 28, weight="bold")
        if subtitle:
            self.text(x + 22, y + 76, subtitle, 21, MUTED)

    def board(self, y=230, h=600, x=75, w=350):
        self.card(x, y, w, h, "ESP32-S3", "พินตาม Lab07", "#edf4fb")
        self.rect(x + 24, y + 100, 122, 75, "#c8d4df", "#8c9eaf", 5)
        self.text(x + 85, y + 145, "S3", 21, anchor="middle")
        self.rect(x + 24, y + h - 65, 82, 42, "#d1dce6", "#8c9eaf", 4)
        self.text(x + 65, y + h - 37, "USB", 20, anchor="middle")

    def wire(self, y, source, target, label="", color=BLUE, left=425, right=1095, resistor=""):
        self.text(left - 20, y + 8, source, 26, anchor="end", weight="bold")
        self.text(right + 20, y + 8, target, 26, weight="bold")
        self.path([(left, y), (right, y)], color)
        self.dot(left, y, color)
        self.dot(right, y, color)
        if resistor:
            mid = (left + right) / 2
            self.rect(mid - 80, y - 22, 160, 44, "#fff9e9", "#a57920", 3)
            self.text(mid, y + 9, resistor, 25, anchor="middle")
        if label:
            self.text((left + right) / 2, y - 24 if not resistor else y - 40, label, 23, color, anchor="middle")

    def notes(self, lines, y=945):
        self.rect(65, y, 1470, 140, "#f6f9fc", "#dce6ef", 12)
        for i, line in enumerate(lines):
            self.text(88, y + 35 + i * 36, line, 23)

    def save(self, lab, name):
        path = ROOT / "labs" / lab / "images" / f"{name}.svg"
        path.write_text("\n".join(self.parts + ["</g></svg>"]), encoding="utf-8")
        print(path.relative_to(ROOT))


def led(lab, name, number):
    d = Diagram(f"LAB {number} | ESP32 → LED", "วงจรภายนอก: GPIO5 → R 220Ω → LED ขา A (+) และ LED ขา K (−) → GND")
    d.board(y=245, h=600)
    d.card(1095, 245, 430, 600, "LED ภายนอก", "R 220Ω ต่ออนุกรมเสมอ", "#fff8f1")
    d.wire(440, "GPIO5", "A / Anode (+)", "สายสัญญาณ → ตัวต้านทาน → LED", BLUE, resistor="220Ω")
    d.wire(635, "GND", "K / Cathode (−)", "สายกลับ GND", BLACK)
    d.path([(1380, 440), (1420, 440), (1420, 505)], BLUE)
    d.add('<path d="M1393 555 V526 a27 27 0 0 1 54 0 V555 Z" fill="#ec6570" stroke="#ad2834" stroke-width="3"/>')
    d.path([(1420, 555), (1420, 635), (1385, 635)], BLACK)
    d.text(1140, 735, "A = ขายาว • K = ขาสั้น/ด้านแบน", 22)
    d.text(1140, 778, "ถ้าตัดขาแล้ว ให้ตรวจขั้วก่อนต่อ", 22)
    if number == "02":
        lines = ["2.1 / 2.4 / 2.5 / 2.6: ใช้ LED ภายนอก GPIO5 ตัวเดิมทุกการทดลอง", "โค้ดเลือกพินตาม target: S3 = GPIO5; WROOM = GPIO16 ไม่ต้องแก้ LED_PIN เอง", "2.2 Serial / 2.3 Log: ใช้ USB data เท่านั้น • ถอดไฟก่อนย้ายสาย"]
    else:
        lines = ["6.3 mqtt_led_control: ใช้ LED ภายนอก GPIO5 ตัวเดิมจาก Lab02–03", "S3 ใช้ LED5 ไม่ใช่ Relay16/17; โค้ดเลือกพินตาม target อัตโนมัติ", "6.1 / 6.2 / 6.4 / 6.5: ใช้ USB และ Wi-Fi • telemetry เป็น uptime/RSSI/heap"]
    d.notes(lines)
    d.save(lab, name)


def buttons():
    d = Diagram("LAB 03 | ESP32-S3 → โมดูลปุ่ม + LED", "ใช้ปุ่ม GND/VCC/OUT ตัวเดิมต่อเนื่องถึง Lab04 • OUT18 / LED5 / LED ดวงที่สอง2")
    d.board(y=230, h=650)
    d.card(1095,230,430,350,"ปุ่มโมดูล 3 พิน","มี R bias ในตัว ใช้ INPUT", "#f0faf7")
    for y,src,dst,col in [(350,"GND","GND",BLACK),(440,"3V3","VCC",ORANGE),(530,"GPIO18","OUT",TEAL)]:
        d.wire(y,src,dst,color=col)
    d.card(1095,625,430,255,"LED ภายนอก","R 220Ω ต่ออนุกรม", "#fff8f1")
    d.wire(740,"GPIO5","A / Anode (+)",resistor="220Ω")
    d.wire(840,"GND","K / Cathode (−)",color=BLACK)
    d.notes(["3.1 ใช้ USB; 3.2/3.3/3.6 ใช้ปุ่ม18+LED5; 3.4 ใช้ปุ่ม18", "3.5 เพิ่ม LED อีกตัว: GPIO2 → R220Ω → A (+), K (−) → GND; S3 ไม่ใช้ LED onboard ในโค้ดนี้", "ตั้ง BUTTON_PRESSED_LEVEL ตาม raw ตอนกด/ปล่อย แล้วใช้ค่าเดียวกันถึง Lab04 ไม่ต้องย้าย OUT18"])
    d.save("lab03-programming-basics", "lab03-wiring-pin-map")


def lab04_modules():
    runpy.run_path(str(ROOT / "scripts" / "generate-lab04-diagrams.py"))["generate_all"]()


def ultrasonic():
    d = Diagram("LAB 05 | ESP32 → Ultrasonic HC-SR04", "TRIG = GPIO6 • ECHO → R1 1kΩ → จุด J → GPIO7 • จุด J → R2 2kΩ → GND")
    d.board(y=230, h=635)
    d.card(1110, 230, 420, 635, "HC-SR04", "โมดูล Ultrasonic 5V", "#f0faf7")
    d.wire(365, "5V rail", "VCC", "ไฟเลี้ยง 5V", RED, right=1110)
    d.wire(465, "GPIO6", "TRIG", "พัลส์สั่งวัด ≥ 10 µs", BLUE, right=1110)
    d.wire(600, "GPIO7", "ECHO", "ECHO → R1 → จุด J → GPIO7", PURPLE, right=1110)
    d.rect(870, 578, 170, 44, "#fff9e9", "#a57920", 3)
    d.text(955, 609, "R1 1kΩ", 24, anchor="middle")
    d.path([(690, 600), (690, 790)], PURPLE)
    d.rect(665, 655, 50, 80, "#fff9e9", "#a57920", 3)
    d.text(740, 708, "R2 2kΩ", 25)
    d.dot(690, 600, PURPLE, 8)
    d.text(690, 641, "จุด J", 22, PURPLE, anchor="end")
    d.wire(790, "GND", "GND", color=BLACK, right=1110)
    d.dot(690, 790, BLACK, 8)
    d.notes(["จุด J มี 3 ทาง: ปลาย R1 + ขา GPIO7 + ปลายบน R2; ปลายล่าง R2 ต่อ GND", "V(J) = 5 × 2k / (1k + 2k) ≈ 3.3V • ห้ามต่อ ECHO 5V เข้า GPIO7 โดยตรง", "5.2 ultrasonic_read / 5.3 filter ใช้ชุดนี้; 5.4 distance_relay ใช้ร่วมกับ Relay16/17"])
    d.save("lab05-relay-ultrasonic", "lab05-ultrasonic-wiring")


def relay():
    d = Diagram("LAB 05 | ESP32 → Relay 2CH → โหลด LED DC", "ใช้โมดูล Relay 5V ที่ input รองรับ 3.3V และไม่ดึง IN ไป 5V • ห้ามใช้ไฟ AC 220V")
    d.board(y=230, h=395)
    d.card(1095, 230, 435, 395, "Relay 2CH", "ตัวอย่าง active LOW", "#f1f7ff")
    d.wire(380, "GPIO16", "IN1", "สัญญาณควบคุม CH1", BLUE)
    d.wire(475, "GPIO17", "IN2", "สัญญาณควบคุม CH2", TEAL)
    d.wire(570, "GND", "GND", "ต่อ GND ร่วมกับแหล่งจ่ายโมดูล", BLACK)
    d.rect(65, 665, 1470, 105, "#fff5ee")
    d.text(90, 704, "ไฟโมดูล: แหล่งจ่าย 5V (+) → Relay VCC | แหล่งจ่าย (−) → Relay GND และ ESP32 GND", 24)
    d.text(90, 747, "ESP32 รับไฟจาก USB; ไม่ต่อแหล่ง 5V ภายนอกเข้าขา 5V ของบอร์ดซ้ำขณะเสียบ USB", 24, RED)
    d.rect(65, 800, 1470, 110, "#f0faf7")
    d.text(90, 833, "+5V DC", 23, RED)
    d.text(365, 833, "COM1", 23)
    d.text(600, 833, "NO1", 23)
    d.path([(90, 855), (400, 855), (560, 829)], RED)
    d.dot(400, 855, RED)
    d.dot(620, 855, RED)
    d.path([(620, 855), (815, 855)], RED)
    d.rect(815, 836, 140, 38, "#fff9e9", "#a57920", 3)
    d.text(885, 863, "R220Ω", 23, anchor="middle")
    d.path([(955, 855), (1025, 855)], RED)
    d.text(1030, 863, "A (+) LED K (−)", 23)
    d.path([(1230, 855), (1350, 855)], BLACK)
    d.text(1370, 863, "DC (−)", 23, BLACK)
    d.text(90, 898, "ภาพแสดงหน้าสัมผัสขณะ OFF: COM1 ยังไม่ถึง NO1 • NC1 เว้นว่าง • CH2 ใช้ COM2/NO2 และ LED+R อีกชุด", 21)
    d.notes(["5.1 relay_basic ใช้ชุด Relay; 5.4 distance_relay เพิ่ม HC-SR04 จากภาพก่อนหน้า", "5V สำหรับ coil ต้องมีกระแสพอ; JD-VCC/jumper และขั้ว COM/NO/NC ตรวจตามคู่มือโมดูลจริง", "COM/NO/NC เป็นหน้าสัมผัสโหลด แยกจาก VCC/GND/IN; ห้ามใช้เป็นขาจ่ายไฟให้ ESP32"])
    d.save("lab05-relay-ultrasonic", "lab05-relay-wiring")


def dht_oled():
    d = Diagram("เตรียม LAB 07 | ESP32 → DHT11 + OLED I2C", "ส่วนต่อยอดท้าย Lab06: OLED8/9 และ DHT4 ใช้พินเดิมถึง Lab07 บน ESP32-S3")
    d.board(y=215, h=680)
    d.card(1110, 215, 420, 300, "DHT11 แบบโมดูล 3 ขา", "เลือกรุ่นรองรับ VCC 3.3V", "#eef7ff")
    d.card(1110, 550, 420, 345, "OLED SSD1306 I2C", "โมดูล 4 ขา รองรับ 3.3V", "#f0faf7")
    for y, src, dst, col in [(335, "3V3", "VCC / +", ORANGE), (405, "GPIO4", "DATA / S / OUT", BLUE), (475, "GND", "GND / −", BLACK), (665, "3V3", "VCC", ORANGE), (735, "GPIO8", "SDA", BLUE), (805, "GPIO9", "SCL", TEAL), (865, "GND", "GND", BLACK)]:
        d.wire(y, src, dst, color=col, right=1110)
    d.notes(["อ่านสกรีนจริง: ลำดับ VCC/DATA/GND ของ DHT และ GND/VCC/SCL/SDA ของ OLED ต่างกันได้", "DHT11 เปล่า 4 ขา: มองด้านตะแกรง ขาชี้ลง → 1 VCC / 2 DATA / 3 NC ไม่ต่อ / 4 GND", "DHT DATA ต้องมี pull-up 10kΩ ไป 3V3 ถ้าโมดูลไม่มีในตัว • คง SDA8/SCL9 และ DATA4 ไปใช้ใน Lab07"])
    d.save("lab06-wifi-mqtt", "lab06-prep-dht11-oled-wiring")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--render-png", action="store_true")
    parser.add_argument("--browser", type=Path)
    args = parser.parse_args()
    if args.render_png and (args.browser is None or not args.browser.is_file()):
        parser.error("--render-png ต้องระบุ --browser เป็น path ของ Chrome/Chromium ที่มีอยู่จริง")
    led("lab02-blink-serial", "lab02-wiring-pin-map", "02")
    buttons()
    lab04_modules()
    ultrasonic()
    relay()
    led("lab06-wifi-mqtt", "lab06-wiring-pin-map", "06")
    dht_oled()
    if args.render_png:
        profile = ROOT / "build" / "wiring-render" / "chrome-cli"
        profile.mkdir(parents=True, exist_ok=True)
        svgs = set(ROOT.glob("labs/lab0[2-6]*/images/*wiring*.svg"))
        svgs.update(ROOT.glob("labs/lab04-lcd-i2c/images/*.svg"))
        for svg in sorted(svgs):
            dimensions = ET.parse(svg).getroot().attrib
            png = svg.with_suffix(".png")
            subprocess.run([
                str(args.browser), "--headless", "--disable-gpu", "--hide-scrollbars",
                "--allow-file-access-from-files", "--disable-background-networking", "--no-first-run",
                "--virtual-time-budget=1000", f"--user-data-dir={profile}",
                f"--screenshot={png}", f"--window-size={dimensions['width']},{dimensions['height']}", svg.as_uri(),
            ], check=True, capture_output=True, timeout=45)
            if not png.is_file():
                raise RuntimeError(f"Browser ไม่สร้าง {png}")
            print(png.relative_to(ROOT))

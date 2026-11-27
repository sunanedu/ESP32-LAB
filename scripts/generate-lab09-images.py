"""Original SVG assets for Lab09; render via Chrome headless, no external artwork."""
import argparse
from pathlib import Path
import runpy
import subprocess
import xml.etree.ElementTree as ET

ROOT = Path(__file__).resolve().parents[1]
LAB = 'lab09-secure-homeguard'
OUT = ROOT/'labs'/LAB/'images'
shared = runpy.run_path(str(ROOT/'scripts/generate-wiring-diagrams.py'))
Diagram = shared['Diagram']


def generate():
    OUT.mkdir(parents=True, exist_ok=True)
    (OUT/'homeguard-icon.svg').write_text('''<svg xmlns="http://www.w3.org/2000/svg" width="1024" height="1024" viewBox="0 0 1024 1024"><title>S3 HomeGuard original app icon</title><rect width="1024" height="1024" rx="220" fill="#087E68"/><path d="M512 218 L756 310 V502 Q756 682 512 806 Q268 682 268 502 V310Z" fill="#E6FFF5"/><path d="M354 481 L512 352 L670 481 M388 455 V633 H636 V455" fill="none" stroke="#087E68" stroke-width="42" stroke-linecap="round" stroke-linejoin="round"/><path d="M483 632 V535 H541 V632" fill="none" stroke="#087E68" stroke-width="30"/></svg>''', encoding='utf-8')

    d = Diagram('LAB 09 | S3 HomeGuard — ชุดอุปกรณ์', 'ใช้ชุดเดิมจาก Lab07 เพิ่มปุ่มเปิดหน้าต่างจับคู่ ไม่ย้ายสาย และใช้ OLED SH1106G')
    for x, title, lines in [
        (65, 'Android / React Native', ['แอปชื่อ S3 HomeGuard', 'Native Kotlin: keys + crypto', 'BLE หรือ Wi-Fi SoftAP', 'กรอกรหัสเอง / QR optional']),
        (575, 'ESP32-S3 + OLED', ['SH1106G SDA8 / SCL9', 'DHT11 DATA4', 'ปุ่มโมดูล OUT18', 'Relay IN1=16 / IN2=17']),
        (1085, 'MQTT broker + MQTTX', ['TLS / AES-GCM payload', 'EMQX public: ทดลองเท่านั้น', 'เปลี่ยน private/custom ได้', 'MQTTX ดู envelope ไม่ถือ key'])]:
        d.card(x, 250, 450, 590, title, 'สัญลักษณ์อธิบาย ไม่ใช่ภาพถ่าย', '#f0faf6')
        for index, line in enumerate(lines):
            d.text(x+28, 430+index*85, line, 23)
    d.notes(['อุปกรณ์: USB data, บอร์ด S3, OLED128×64, DHT11, Relay2CH, ปุ่ม3พิน, โทรศัพท์ Android',
             'จับคู่ครั้งแรกต้องอยู่ใกล้และกดปุ่มที่บอร์ด; มีสอง transport ไม่บังคับกล้อง',
             'ไม่มีเว็บกลาง; คีย์อยู่ที่โทรศัพท์/ESP ส่วน broker ส่งต่อข้อความเข้ารหัส'])
    d.save(LAB, 'lab09-equipment')

    d = Diagram('LAB 09 | ESP32-S3 — ต่อพินเดิม', 'OLED SH1106G / DHT11 / Relay2CH / ปุ่มโมดูล • หมายเลข GPIO ไม่ใช่ตำแหน่ง header')
    d.board(y=230, h=650)
    d.card(1095,230,435,650,'พินปลายทาง','ตรวจสกรีนแต่ละโมดูลก่อนต่อ')
    rows = [('3V3','OLED VCC','#d76a12'),('GND','OLED GND','#293544'),
            ('GPIO8','OLED SDA','#1769cb'),('GPIO9','OLED SCL','#8555b5'),
            ('GPIO4','DHT11 DATA','#1769cb'),('GPIO16','Relay IN1','#008575'),
            ('GPIO17','Relay IN2','#8555b5'),('GPIO18','Button OUT','#1769cb'),
            ('3V3','DHT / Button VCC','#d76a12'),('GND','DHT/Button/Relay GND','#293544'),
            ('5V supply','Relay VCC','#d83a42')]
    for index, (source, target, color) in enumerate(rows):
        d.wire(350+index*46, source, target, color=color)
    d.notes(['ปุ่มมี R bias ในโมดูล ใช้ INPUT; DHT DATA ต้องมี pull-up10kΩ ถ้าโมดูลไม่มี',
             'Relay ต้องรับลอจิก3.3V ต่อ GND ร่วมและใช้แหล่ง5Vตามคู่มือ ไม่ป้อนไฟย้อนบอร์ด USB',
             'preflight คง Relay OFF; ถอดโหลดก่อนทดสอบ ใช้เฉพาะโหลด DC แรงดันต่ำ ห้าม AC220V'])
    d.save(LAB, 'lab09-wiring')

    d = Diagram('LAB 09 | เป้าหมายหน้าแอปและการจับคู่', 'ภาพจำลองแนวทาง UI ไม่ใช่ภาพ Android ที่ทดสอบแล้ว • แอป starter ยังล็อก Relay')
    d.card(95,220,650,660,'S3 HomeGuard','Advanced Security IoT Smart Home','#f0faf6')
    for y, label in [(370,'Broker connected ≠ Device authenticated'),(455,'Relay 1 / 2: UNKNOWN — ล็อก'),(540,'วิธีที่ 1: Bluetooth LE + Security2'),(625,'วิธีที่ 2: Wi-Fi SoftAP + Security2'),(710,'อ่าน SRP credential จาก OLED / Serial'),(795,'กรอกเองได้ ไม่บังคับสแกน QR')]:
        d.text(125,y,label,24)
    d.card(805,220,680,660,'ลำดับทำงาน','ทุกวิธีใช้ physical pairing window120s','#f6f9fc')
    for index, line in enumerate(['1  กดปุ่มที่บอร์ดค้าง 2 วินาที','2  เลือก BLE หรือ SoftAP ในแอป','3  ตรวจอุปกรณ์ / กรอก credential','4  Secure handshake → ส่ง config+key','5  ปิด pairing / ต่อ MQTT ผ่าน TLS','6  ยืนยัน session → จึงปลดล็อก control']):
        d.text(835,370+index*85,line,26)
    d.notes(['รหัสบนจอใน preflight เป็น DEMO เท่านั้น ยังไม่มี BLE/SoftAP endpoint หรือการส่งคีย์',
             'runtime จริงต้องใช้ username/password ของ Security2 ที่มี entropy สูง ไม่ใช้ PIN6หลักเป็น root',
             'การเปลี่ยน broker ต้องผ่าน stage/test/confirm/commitทั้งสองฝั่ง ไม่ลดระดับ TLS'])
    d.save(LAB, 'lab09-pairing-ui')


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--render-png', action='store_true')
    args = parser.parse_args()
    generate()
    if args.render_png:
        browser = Path('C:/Program Files/Google/Chrome/Application/chrome.exe')
        profile = ROOT/'build/lab09-qa/chrome'
        profile.mkdir(parents=True, exist_ok=True)
        for svg in sorted(OUT.glob('*.svg')):
            size = ET.parse(svg).getroot().attrib
            subprocess.run([str(browser), '--headless', '--disable-gpu', '--hide-scrollbars',
                            '--allow-file-access-from-files', '--disable-background-networking', '--no-first-run',
                            '--virtual-time-budget=1000', f'--user-data-dir={profile}',
                            f'--screenshot={svg.with_suffix(".png")}',
                            f'--window-size={size["width"]},{size["height"]}', svg.as_uri()],
                           check=True, capture_output=True, timeout=45)
            print(svg.with_suffix('.png').relative_to(ROOT))

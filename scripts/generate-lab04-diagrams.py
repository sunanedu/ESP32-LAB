"""ภาพ OLED/ปุ่ม/ไฟ RD-YL-GY ของ Lab04; เรียกผ่าน generate-wiring-diagrams.py."""
from pathlib import Path
import runpy

ROOT = Path(__file__).resolve().parents[1]
shared = runpy.run_path(str(ROOT / "scripts" / "generate-wiring-diagrams.py"))
Diagram = shared["Diagram"]
BLUE, TEAL, ORANGE, BLACK = (shared[k] for k in ("BLUE", "TEAL", "ORANGE", "BLACK"))
RED, PURPLE = shared["RED"], shared["PURPLE"]
LAB = "lab04-lcd-i2c"


def oled():
    d = Diagram("LAB 04 | ESP32 → OLED I2C ประมาณ 1 นิ้ว", "ชุดเรียนใช้ SSD1306 / 128×64 / 0x3C เป็นค่าเริ่มต้น • ตรวจ controller และ address ของจอจริง")
    d.board(y=230, h=650)
    d.card(1095, 230, 435, 650, "OLED I2C 4 พิน", "รุ่นที่รองรับ VCC 3.3V", "#effaf7")
    for y, src, dst, color in [(410,"3V3","VCC",ORANGE),(530,"GND","GND",BLACK),(650,"GPIO8","SDA",BLUE),(770,"GPIO9","SCL",TEAL)]:
        d.wire(y,src,dst,color=color)
    d.notes(["4.1 Scanner / 4.2 OLED test: ต่อ 4 สายนี้ • SDA คือข้อมูล / SCL คือ clock", "อ่านสกรีนจริง: บางโมดูลเรียง GND/VCC/SCL/SDA ต่างกัน • สาย I2C ต้องเป็นระดับ 3.3V", "พบ ACK ที่ 0x3C/0x3D ยังไม่ยืนยันชนิดจอ; ถ้าเป็น SH1106 ต้องเปลี่ยน driver ให้ตรงรุ่น"])
    d.save(LAB,"lab04-oled-wiring")


def button():
    d = Diagram("LAB 04 | ESP32 → โมดูลปุ่มกด 3 พิน", "พินบนโมดูล: GND / VCC / OUT • มี R bias ในตัว • ต่อไฟและกราวด์ด้วย แม้มีสายสัญญาณเส้นเดียว")
    d.board(y=230,h=650)
    d.card(1095,230,435,650,"โมดูล Push switch","รุ่นรองรับไฟ/OUT 3.3V", "#f0faf7")
    for y, src, dst, color in [(410,"GND","GND",BLACK),(560,"3V3","VCC",ORANGE),(710,"GPIO18","OUT",TEAL)]:
        d.wire(y,src,dst,color=color)
    d.notes(["4.3 button test: ใช้ pinMode(18, INPUT) เพราะโมดูลมี R ในตัว ไม่เพิ่ม R ภายนอก", "อ่าน raw ตอนปล่อย/กดก่อน: ตั้ง BUTTON_PRESSED_LEVEL = LOW หรือ HIGH ให้ตรงรุ่น", "เป็นปุ่มชั่วขณะ: ปล่อยแล้วคืนสถานะ; debounce ในโค้ดนับ 1 ครั้งต่อการกดที่นิ่ง"])
    d.save(LAB,"lab04-button-module-wiring")


def traffic():
    d = Diagram("LAB 04 | ESP32 → โมดูลไฟ 8 mm สามสี", "พินตามผู้ใช้: VCC / RD / YL / GY • RD แดง / YL เหลือง / GY เขียว • ขั้วบวกร่วม")
    d.board(y=230,h=650)
    d.card(1095,230,435,650,"โมดูล VCC/RD/YL/GY","ใช้รุ่น 3.3V ที่มี R จำกัดกระแส", "#fff8f1")
    for y,src,dst,color in [(410,"3V3","VCC",ORANGE),(530,"GPIO10","RD (แดง)",RED),(650,"GPIO11","YL (เหลือง)","#a47e00"),(770,"GPIO12","GY (เขียว)",TEAL)]:
        d.wire(y,src,dst,color=color)
    d.notes(["4.4 traffic test: common VCC → LOW = เปิด / HIGH = ดับ เป็นค่าเริ่มต้นที่ปรับได้", "โมดูลนี้มี 4 สาย ไม่มีขา GND แยก: ช่องสีไหลกลับผ่าน GPIO เมื่อ LOW; ห้ามสลับ VCC เป็น GND", "ต้องยืนยัน R/กระแสต่อช่องก่อนต่อ GPIO; ถ้าเป็นรุ่น 5V หรือกระแสเกิน ให้ใช้วงจรขับตามรุ่น"])
    d.save(LAB,"lab04-traffic-light-wiring")


def integrated():
    d = Diagram("LAB 04 | รวม OLED + ปุ่มโมดูล + ไฟ RD/YL/GY", "ESP32-S3 ตาม Lab07 • ใช้ไฟ 3.3V • พินสัญญาณไม่ชน HC-SR04 / Relay / DHT11 ของแล็บอื่น", height=1400)
    d.board(y=220,h=945)
    d.card(1095,220,435,320,"OLED I2C","SSD1306 เป็น baseline", "#effaf7")
    d.card(1095,570,435,250,"ปุ่ม 3 พิน","มี R bias ในตัว", "#eef7ff")
    d.card(1095,850,435,315,"ไฟสามสี 8 mm","เลือกรุ่นมี R จำกัดกระแสในตัว", "#fff8f1")
    rows=[(340,"3V3","VCC",ORANGE),(400,"GND","GND",BLACK),(460,"GPIO8","SDA",BLUE),(520,"GPIO9","SCL",TEAL),
          (670,"GND","GND",BLACK),(730,"3V3","VCC",ORANGE),(790,"GPIO18","OUT",PURPLE),
          (960,"3V3","VCC",ORANGE),(1020,"GPIO10","RD (แดง)",RED),(1080,"GPIO11","YL (เหลือง)","#a47e00"),(1140,"GPIO12","GY (เขียว)",TEAL)]
    for y,src,dst,col in rows:
        d.wire(y,src,dst,color=col)
    d.notes(["4.5 button+OLED: ใช้จอและปุ่ม • 4.6 รวมระบบ: ใช้ครบสามโมดูลตามรูปนี้", "3V3/GND ที่เขียนซ้ำหมายถึงแบ่งจากจุดเดียวกันผ่านราง Breadboard ไม่ใช่ต้องมีหลายพิน", "คง OLED8/9 ไปถึง Lab07; ไม่ชน Relay16/17 • ขาที่สเก็ตช์ไม่ได้กำหนดอาจลอย"],y=1200)
    d.save(LAB,"lab04-integrated-wiring")


def pinout():
    d = Diagram("LAB 04 | อ่านชื่อพินก่อนต่อสาย", "ภาพเชิงอธิบายจากสกรีนที่ผู้ใช้แจ้ง • ตำแหน่งขา/สี PCB/ตัวต้านทานต้องตรวจโมดูลจริง")
    cards=[(65,"OLED I2C 4 พิน","GND / VCC / SCL / SDA"),(570,"โมดูลปุ่มกด","GND / VCC / OUT"),(1075,"โมดูลไฟ 8 mm","VCC / RD / YL / GY")]
    for x,title,pins in cards:
        d.card(x,230,460,630,title,pins,"#f4f8fc")
    d.rect(100,360,390,190,"#153c6a","#274c6a",5)
    d.text(295,430,"OLED 128 x 64",31,"#fff",anchor="middle")
    d.text(295,485,"SSD1306? ตรวจรุ่น",25,"#b4eaff",anchor="middle")
    d.text(100,620,"GND → ESP32 GND",25)
    d.text(100,670,"VCC → ESP32 3V3",25)
    d.text(100,720,"SCL → GPIO9",25)
    d.text(100,770,"SDA → GPIO8",25)
    d.rect(675,360,250,190,"#3e7658","#29503b",6)
    d.add('<circle cx="800" cy="455" r="60" fill="#344252" stroke="#17283f" stroke-width="4"/>')
    d.text(605,620,"GND → ESP32 GND",25)
    d.text(605,670,"VCC → ESP32 3V3",25)
    d.text(605,720,"OUT → GPIO18",25)
    d.text(605,790,"R bias มีอยู่ในโมดูลแล้ว",23)
    for x,col in [(1160,RED),(1305,"#e9bf27"),(1450,TEAL)]:
        d.add(f'<circle cx="{x}" cy="455" r="45" fill="{col}" stroke="#17283f" stroke-width="3"/>')
    for y,line in [(620,"VCC → ESP32 3V3"),(670,"RD → GPIO10 (แดง)"),(720,"YL → GPIO11 (เหลือง)"),(770,"GY → GPIO12 (เขียว)")]:
        d.text(1110,y,line,25)
    d.notes(["ไฟ RD/YL/GY คือแดง–เหลือง–เขียว ไม่ใช่ชุด R/G/B ที่มีสีน้ำเงิน", "common VCC ใช้ LOW เปิดเป็น baseline; ต้องมี R จำกัดกระแสในโมดูลและรองรับการขับด้วย GPIO", "ทั้งสามโมดูลใช้รุ่นรองรับ 3.3V • ลำดับพินจริงให้ยึดตัวอักษรสกรีน ไม่เดาจากภาพนี้"])
    d.save(LAB,"lab04-module-pinout")


def result():
    d = Diagram("LAB 04 | ตัวอย่างผลลัพธ์ OLED", "ภาพจำลองที่สร้างจากรูปแบบผลลัพธ์ในโค้ด • ไม่ใช่ภาพถ่ายจอหรือหลักฐานทดสอบฮาร์ดแวร์")
    d.card(65,235,700,640,"4.2 ทดสอบจอ","128×64 pixel / มุมซ้ายบน (0,0)")
    d.card(800,235,735,640,"4.6 รวมสามโมดูล","กดปุ่มหนึ่งครั้ง → เปลี่ยนสถานะหนึ่งขั้น")
    for x in [100,835]:
        d.rect(x,365,630,315,"#101b27","#17283f",4)
    for y,text in [(420,"Lab04 OLED test"),(485,"Hello!"),(550,"Uptime: 0 s"),(615,"Frame: 1")]:
        d.text(125,y,text,32,"#d3f5ff")
    for y,text in [(420,"Lab04 Traffic"),(485,"GREEN"),(615,"Presses: 3")]:
        d.text(860,y,text,32,"#d3f5ff")
    d.text(100,758,"clearDisplay() ล้าง buffer ใน RAM",26)
    d.text(100,812,"display() ส่ง buffer ไปให้จอแสดงผล",26)
    d.text(835,758,"OFF → RED → YELLOW → GREEN → OFF",26)
    d.text(835,812,"กดค้างไม่เพิ่มซ้ำจนกว่าจะปล่อยแล้วกดใหม่",24)
    d.notes(["ข้อความ/ตำแหน่งเป็นตัวอย่างคาดหวัง ต้องเทียบผล Serial กับจอจริงเมื่อทดสอบ", "ASCII บนจอใช้ font เริ่มต้นของ Adafruit GFX; ยังไม่ได้ติดตั้ง font ภาษาไทย", "จอตอบ ACK แต่ภาพไม่ถูก: ตรวจ driver/ความละเอียด/address ไม่สรุปจากขนาดจอประมาณ 1 นิ้ว"])
    d.save(LAB,"lab04-oled-result")


def generate_all():
    for draw in (pinout,oled,button,traffic,integrated,result):
        draw()


if __name__ == "__main__":
    generate_all()

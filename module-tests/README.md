# โค้ดทดสอบแยกโมดูล — ESP32-S3

ชุดนี้มี 6 สเก็ตช์แยกกัน หากดาวน์โหลดเป็น ZIP ให้แตกไฟล์ก่อนเปิดใน Arduino IDE แต่ละโฟลเดอร์มี `.ino` ชื่อเดียวกัน ใช้ได้โดยไม่คัดลอกโค้ดจากบทเรียนและไม่ต้องมีไฟล์ `secrets.h` เลือกเปิดเพียงสเก็ตช์ของโมดูลที่จะทดสอบ การ Upload จะแทนที่โปรแกรมเดิมบนบอร์ด

ใช้พินคงที่ตาม [ตารางของคอร์ส](https://github.com/sunanedu/ESP32-LAB/blob/main/assets/PINOUT.md) และไม่แก้ Lab07 ชุดนี้ตั้งใจใช้กับ **ESP32-S3** ถ้าเลือก Board เป็น ESP32 รุ่นเดิม โค้ดจะหยุด compile พร้อมแจ้งให้เลือกบอร์ด S3

| โมดูล / เปิดไฟล์ | สายสัญญาณ ESP32-S3 | ไฟเลี้ยง/กราวด์ | การทดสอบ |
|---|---|---|---|
| [DHT11](esp32s3_dht11_test/esp32s3_dht11_test.ino) | DATA/S/OUT → GPIO4 | VCC→3V3, GND→GND | อ่านอุณหภูมิและความชื้นทุก 2 วินาที; รายงาน READ FAILED ถ้าอ่านไม่ได้ |
| [HC-SR04 Ultrasonic](esp32s3_ultrasonic_test/esp32s3_ultrasonic_test.ino) | TRIG→GPIO6; ECHO ผ่าน divider→GPIO7 | VCC→5V, GND ร่วม | แสดงระยะทุก 0.5 วินาที; รายงาน TIMEOUT เมื่อไม่พบ ECHO |
| [ไฟสามสี 8 mm](esp32s3_traffic_light_test/esp32s3_traffic_light_test.ino) | RD→GPIO10, YL→GPIO11, GY→GPIO12 | VCC→3V3; ไม่มี GND แยก | วน OFF→GREEN→YELLOW→RED→ALL ทุก 2 วินาที |
| [Relay 2CH](esp32s3_relay_2ch_test/esp32s3_relay_2ch_test.ino) | IN1→GPIO16, IN2→GPIO17 | VCC→แหล่งจ่ายโมดูล 5V, GND ร่วม | วนปิดทั้งคู่→CH1→CH2→เปิดทั้งคู่ ทุก 2 วินาที |
| [โมดูลปุ่ม GND/VCC/OUT](esp32s3_button_module_test/esp32s3_button_module_test.ino) | OUT→GPIO18 | VCC→3V3, GND→GND | แสดง raw/stable ทุก 0.5 วินาที; นับการกดหลังนิ่ง 40 ms |
| [OLED SH1106/SH1106G I2C](esp32s3_oled_test/esp32s3_oled_test.ino) | SDA→GPIO8, SCL→GPIO9 | VCC→3V3, GND→GND | แสดงกรอบ/ข้อความคงที่และ uptime ทุกวินาที; เลือก SSD1306 ได้ในไฟล์เดียวกัน |

## เปิดและทดสอบ

1. ถอด USB/แหล่งจ่ายก่อนต่อสาย ต่อโมดูลที่จะทดสอบตามตาราง ตรวจสกรีน VCC/GND/OUT และแรงดันของรุ่นจริง
2. เปิด `.ino` **ภายในโฟลเดอร์ชื่อเดียวกัน** ไม่วางทั้ง 6 ไฟล์รวมในโฟลเดอร์ sketch เดียว เพราะแต่ละไฟล์มี `setup()`/`loop()` ของตน
3. เลือก **ESP32S3 Dev Module**, Arduino-ESP32 core **3.x**, COM ของบอร์ด และ Upload Speed **115200** ถ้าใช้ USB-to-UART CH343 ตามบอร์ดผู้สอน ให้ตั้ง **USB CDC On Boot = Disabled** เลือก Flash/PSRAM ตามบอร์ดจริง
4. DHT11 ติดตั้ง **DHT sensor library by Adafruit** และ **Adafruit Unified Sensor**; OLED ของผู้สอนเป็น **SH1106/SH1106G** ให้ติดตั้ง **Adafruit SH110X**, **Adafruit GFX Library** และ **Adafruit BusIO** จาก Library Manager อีก 4 สเก็ตช์ใช้เพียง Arduino ที่มากับ core ถ้าเปลี่ยนไปทดสอบจอ SSD1306 ให้ติดตั้ง **Adafruit SSD1306** และเลือก driver ตามหัวข้อด้านล่าง
5. กด Verify แล้ว Upload เปิด Serial Monitor **115200 baud** และกด EN/Reset เพื่อดูข้อความเริ่มต้น
6. เปรียบเทียบผล Serial กับอุปกรณ์จริง เช่น ระยะเทียบไม้บรรทัด, สีไฟทีละช่อง, การกด–ปล่อย 5 ครั้งและกดค้าง ค่า Serial ของ Relay เป็นค่าที่โปรแกรมสั่ง ต้องตรวจหน้าสัมผัส/โหลด DC แยกจึงยืนยันผลจริงได้

## รายละเอียดการต่อที่ต้องครบ

- **DHT11:** เลือกรุ่นรองรับ 3.3V ถ้าไม่มี pull-up ในตัวให้ต่อ R10kΩ ระหว่าง DATA กับ 3V3 ตัว sensor เปล่า 4 ขาให้หันหน้าตะแกรงเข้าหาผู้มอง ขาชี้ลง: ซ้ายไปขวา `VCC/DATA/NC/GND` โดย NC ไม่ต่อ
- **HC-SR04:** ต่อ `ECHO → R1 1kΩ → จุด J → GPIO7` และ `จุด J → R2 2kΩ → GND` ห้ามต่อ ECHO 5V ตรงเข้าขา ESP32
- **ไฟสามสี:** พินตามโมดูลผู้สอนคือ `VCC/RD/YL/GY` (แดง/เหลือง/เขียว) โค้ดใช้ขั้วบวกร่วม LOW=เปิด ต้องเป็นรุ่น 3.3V ที่มี R จำกัดกระแสและกระแสเหมาะกับ GPIO ตรวจคู่มือวงจรก่อนเปลี่ยน `ACTIVE_LOW`; ไม่สลับ VCC เป็น GND
- **Relay:** ใช้รุ่นที่ IN รองรับ 3.3V และไม่ดึงขา GPIO ขึ้น 5V ต่อ JD-VCC ตามคู่มือ เริ่มโดยถอดโหลดออกจาก COM/NO/NC ก่อน ถ้าทดสอบโหลดให้ใช้ DC แรงดันต่ำ **ห้ามใช้ AC 220V** ไม่ป้อนไฟจากแหล่งจ่าย 5V ภายนอกกลับบอร์ดที่รับไฟ USB
- **ปุ่ม:** มี R bias ในโมดูล ใช้ `INPUT` และต่อ GND/VCC/OUT ครบ ไม่เพิ่ม R ภายนอก จด raw ตอนปล่อยและกด ถ้ากดแล้ว raw=1 ให้เปลี่ยน `BUTTON_PRESSED_LEVEL` จาก `LOW` เป็น `HIGH` และ Upload ใหม่ ใช้ค่าเดียวกันเมื่อกลับไปทำ Lab03/04

- **OLED:** ใช้จอ SH1106/SH1106G I2C 128×64 ที่รองรับ 3.3V ต่อ SDA8/SCL9 โค้ดตรวจ address 0x3C ก่อน 0x3D หากไม่มี ACK จะแจ้งทาง Serial เมื่อเริ่มได้จะแสดงกรอบครบสี่ด้าน ชื่อ driver, address, `ABC 0123456789` และ uptime ไม่มีแถบ DEMO หรือคำสั่งเลื่อนภาพ ACK ยืนยันว่ามีอุปกรณ์ตอบที่ address นั้น แต่ไม่ระบุรุ่น driver และไม่ยืนยันว่าภาพถูกต้อง

## เลือก driver ของ OLED

ไฟล์ [esp32s3_oled_test.ino](esp32s3_oled_test/esp32s3_oled_test.ino) ตั้งต้นเป็น **SH1106/SH1106G** ตามรุ่นที่ผู้สอนยืนยัน ต่อสายเดิมทั้งสี่เส้น:

| ESP32-S3 | พินบน OLED |
|---|---|
| 3V3 | VCC |
| GND | GND |
| GPIO8 | SDA |
| GPIO9 | SCL |

ตรวจชื่อพินบนโมดูลก่อนต่อ เพราะลำดับซ้าย–ขวาของแต่ละร้านอาจต่างกัน

| Controller | Library / คลาส | การเลือกในไฟล์นี้ |
|---|---|---|
| **SH1106 / SH1106G 128×64** | Adafruit SH110X / `Adafruit_SH1106G` | ค่าเริ่มต้น `OLED_DRIVER_SH1106` |
| SSD1306 128×64 | Adafruit SSD1306 / `Adafruit_SSD1306` | เปลี่ยนเป็น `OLED_DRIVER_SSD1306` |
| SH1107 | Adafruit SH110X / `Adafruit_SH1107` | เป็นอีกคลาสหนึ่ง ยังไม่มีตัวเลือกในไฟล์นี้ ต้องตรวจความละเอียดและการวางจอของโมดูลก่อน |

สำหรับจอผู้สอนไม่ต้องแก้ตัวเลือก หากต้องเทียบกับจอ **SSD1306 ที่ยืนยันรุ่นแล้ว** ให้เปลี่ยนเพียงบรรทัดนี้ แล้ว Verify/Upload ใหม่:

```cpp
#define OLED_DRIVER OLED_DRIVER_SSD1306
```

เมื่อต้องการกลับ SH1106 ให้ใช้:

```cpp
#define OLED_DRIVER OLED_DRIVER_SH1106
```

โค้ดเลือก driver ตอน compile และตรวจ address ตอนเริ่มทำงาน ไม่มีการวนส่งคำสั่งหลาย driver สลับกันอัตโนมัติ สีฟ้าหรือ address 0x3C เพียงอย่างเดียวใช้ระบุ controller ไม่ได้

หลัง Upload เปิด Serial Monitor 115200 แล้วกด EN/Reset ตรวจว่าเห็น `driver=SH1106G` และกรอบ/ข้อความครบ ข้อความต้องอยู่กับที่ มีเพียงค่า uptime เปลี่ยนทุกวินาที รูปทดสอบนี้ใช้เปรียบเทียบทั้งสอง driver เหมือนกัน

| อาการ | จุดตรวจและวิธีแก้ |
|---|---|
| `Adafruit_SH110X.h: No such file or directory` | ติดตั้ง **Adafruit SH110X by Adafruit** และ dependencies ใน Library Manager |
| `NO ACK at 0x3C/0x3D` | ถอดไฟก่อนตรวจ VCC/GND/SDA8/SCL9 และให้มีจอ I2C ตัวเดียวระหว่างทดสอบ |
| ACK/init OK แต่ยังเป็นเส้นหรือภาพเลื่อน | ตรวจ Serial ว่าใช้ `SH1106G` และ Upload ไฟล์ฉบับนี้แล้ว จากนั้นถอดไฟแล้วจ่ายใหม่ ACK อย่างเดียวไม่ยืนยัน driver |
| กรอบหรือข้อความขาดเป็นส่วน | ยืนยันความละเอียด 128×64 จากรุ่นจริง ไม่ใช้ขนาดประมาณหนึ่งนิ้วแทนข้อมูลความละเอียด |
| ภาพกลับหัวแต่ข้อความครบ | ปรับ `display.setRotation(0)` เป็น `display.setRotation(2)` แล้ว Upload ใหม่ |
| ใช้ SH1106 ถูกแล้วแต่ภาพยังผิด | โค้ดใช้ I2C 100 kHz อยู่แล้ว ตรวจสายให้สั้น/แน่นและไฟเลี้ยงตามคู่มือโมดูล แล้วทดสอบกับจอหรือบอร์ดอีกตัวเพื่อแยกสาเหตุ |

อ้างอิง API จาก [Adafruit SH110X](https://github.com/adafruit/Adafruit_SH110x), [ตัวอย่าง SH1106 I2C 128×64 ของ Adafruit](https://github.com/adafruit/Adafruit_SH110x/blob/master/examples/OLED_QTPY_SH1106/SH1106_128x64_i2c_QTPY/SH1106_128x64_i2c_QTPY.ino) และ [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306) ตัวอย่างภายนอกอ้างอิง API เท่านั้น ให้ใช้พิน S3 ตามตารางข้างต้น บทเรียน Lab04/07/08 เดิมยังใช้ตัวอย่าง SSD1306; ให้ใช้ไฟล์ทดสอบนี้เมื่อตรวจจอ SH1106 ของผู้สอน

สเก็ตช์เหล่านี้ควบคุมเฉพาะโมดูลที่ระบุ อุปกรณ์อื่นที่ต่อค้างอาจมีขาลอย รวมถึงช่วง Reset/ก่อน setup จึงไม่รับรองสถานะ OFF ของทุกโมดูลบนบอร์ดจากการทดสอบไฟล์ใดไฟล์หนึ่ง

## ตัวอย่างผล Serial ที่คาดหวัง

ตัวเลขเป็นตัวอย่างสำหรับเทียบรูปแบบ ไม่ใช่ข้อมูลที่วัดจากฮาร์ดแวร์จริง

```text
[DHT11] temperature=28.0 C humidity=65.0 %
[ULTRASONIC] echo=1166 us distance=20.0 cm
[LIGHT] GREEN | RD=GPIO10 YL=GPIO11 GY=GPIO12
[RELAY] CH1=ON CH2=OFF | IN1=GPIO16 IN2=GPIO17
[BUTTON] PRESSED count=1
[OLED] driver=SH1106G ACK=0x3C init=OK SDA=8 SCL=9
[OLED] driver=SH1106G uptime=1 s; ตรวจภาพบนจอจริง
```

เฉพาะไฟล์ที่ Upload อยู่เท่านั้นจะส่ง log ของตน ไม่ได้รันทั้งหกโมดูลพร้อมกัน

## ตรวจ build

ใช้ Arduino CLI กับ core 3.x และ library ของโมดูลที่ติดตั้งแล้ว ตัวอย่างบอร์ดผู้สอน:

```powershell
arduino-cli compile --fqbn "esp32:esp32:esp32s3:FlashSize=16M,PSRAM=opi,CDCOnBoot=default,UploadSpeed=115200" --warnings all module-tests/esp32s3_dht11_test
```

เปลี่ยน path ท้ายคำสั่งเป็นโฟลเดอร์ของสเก็ตช์ที่จะตรวจ ค่า Flash/PSRAM ต้องตรงรุ่นจริง ส่วนการ compile ไม่ได้ยืนยันการต่อสายหรือผลอ่านเซ็นเซอร์บนอุปกรณ์จริง

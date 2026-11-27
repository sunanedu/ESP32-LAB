# Prompt — ESP32-WROOM-32 / DHT11 module / OLED

ใช้ built-in image_gen (generate แล้ว edit เฉพาะส่วน DHT11); ภาพประกอบสร้างด้วย AI ตาม config.h ไม่ใช่ภาพทดสอบโมดูลจริง
ภาพสุดท้ายใช้โมดูล DHT11 3 ขาที่มี pull-up บน PCB แยกจากภาพ pin guide ที่อธิบาย sensor เปล่า4ขา ขาในกล่องเป็น functional map ไม่ใช่ตำแหน่ง header จริง

## Prompt สุดท้าย

```text
Use case: precise-object-edit. Input Image1 is the wiring poster to edit. Input Image2 is a supporting reference ONLY for the three-pin DHT11 breakout in its middle panel.
Keep Image1's title, two-panel white composition, both ESP32-WROOM-32 board illustrations, all text in the RIGHT OLED panel, and ALL FOUR right-panel OLED wires EXACTLY unchanged. Right panel is already correct: GND black,3V3 orange,GPIO22/SCL purple,GPIO21/SDA blue.
Change ONLY the LEFT DHT11 circuit and its small bottom footnote. Replace the bare four-leg blue sensor with a THREE-PIN blue DHT11 on a small breakout PCB like Image2's center module. The PCB has exactly THREE visible connector terminals, left-to-right VCC, DATA, GND, with a clear printed label beside each pad and an onboard pull-up resistor. Do not show any bare four-pin DHT11 legs or NC label in this wiring poster.
In the left panel put this three-terminal module at TOP and the ESP32-WROOM-32 illustration below. The magnified Functional GPIO map above the bottom board has three clearly visible pads in left-to-right order 3V3, GPIO4, GND. Align top module VCC exactly vertically over 3V3, DATA over GPIO4, GND over GND. Three continuous vertical wires join pad-to-pad: ORANGE VCC to3V3, GREEN DATA toGPIO4, BLACK GND toGND. All three must physically TOUCH their source and destination terminal pads, with NO gaps or labels breaking a wire. All three wires stay parallel and separate, never cross. Put pin labels beside their pads, not in the middle of wires. No fourth terminal or missing wire. No external resistor because this example breakout has its pull-up on its PCB; label "3-PIN MODULE • onboard pull-up".
Remove the footer "DHT11 pin3 NC". Replace with "Example 3-pin breakout • Check module labels and onboard pull-up". Preserve the footer that says both views connect to the SAME ESP32-WROOM-32 and share 3V3/GND. Preserve the color legend. No other edits; especially do not change the right OLED pin order, mapping or wires.
```

## ข้อกำหนดภาพเมื่อสร้างใหม่

- พื้นขาวสองชุดสายของบอร์ดเดียว: DHT11 สามขาใช้ VCC→3V3, DATA→GPIO4, GND→GND; ตัวอย่างมี pull-up ในโมดูล
- OLED4ขา GND/VCC/SCL/SDA ต่อ GND/3V3/GPIO22/GPIO21 เป็นสายขนานไม่ตัดกัน
- สีส้ม=3.3V, ดำ=GND, เขียว=DATA, น้ำเงิน=SDA, ม่วง=SCL; ให้ปลายสายแตะ pad ทุกเส้น ไม่ใช้ลูกศรแทนสาย
- มองสองรูปเป็นบอร์ดเดียวกัน ใช้ราง3V3/GNDร่วม; ภาพ sensor เปล่า4ขาอยู่ใน lab07-dht11-oled-pinout.md

แหล่งตรวจ: [config.h](../../labs/lab07-smart-home-project/code/lab07_smart_home/config.h), [Adafruit DHT wiring](https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor)


## Prompt ตรวจปลายสายรอบสุดท้าย

```text
Use case: precise-object-edit. Edit ONLY the left DHT11 panel's lower Functional GPIO map strip and the last part of its three wires. Preserve ALL other pixels/composition, especially the entire correct right OLED panel, board names, module, headings, footer and legend. The LEFT strip must have exactly THREE columns, left-to-right 3V3, GPIO4, GND. Put a large circular electrical terminal PAD on the TOP EDGE of each column, with its label printed BELOW that pad INSIDE its column. Place the phrase Functional GPIO map BELOW the strip as a caption, never between the module and the pads. Route the orange VCC wire to the LEFT TOP PAD labeled3V3, using a short dogleg to the LEFT if needed. Route the green DATA wire to the MIDDLE TOP PAD labeledGPIO4. Route the black GND wire to the RIGHT TOP PAD labeledGND, using a short dogleg to the RIGHT if needed. Three separate wires, no touching/crossing. ALL THREE wires must visibly TOUCH the center of their matching circular pads on the top edge, not a header, text or rectangle border. No gap, no incorrect column endpoint. Labels below their matching pads must stay unobscured by wires. This is a functional named terminal strip, not a physical board pinout, so no need to draw small colored segments from the bottom of the strip to a guessed ESP32 physical pin; remove such little segments if present. The actual sensor module VCC/DATA/GND connector and right OLED circuit remain unchanged.
```

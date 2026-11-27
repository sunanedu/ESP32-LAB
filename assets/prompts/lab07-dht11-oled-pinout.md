# Prompt — ดูขา DHT11 / OLED และเลือก GPIO ให้ตรงบอร์ด

ใช้ built-in image_gen; สรุป pinout ของอุปกรณ์และขาที่โค้ดใช้ก่อนต่อวงจร
ลำดับขา breakout 3 ขาและ OLED เป็นตัวอย่างที่มีสกรีนกำกับ ไม่ถือเป็นลำดับสากลของทุกโมดูล

## Prompt

```text
Use case: scientific-educational.
Create ONE clean high-resolution landscape electronics pin identification infographic, white background, razor-sharp large dark sans serif labels, realistic top/front component illustrations with light flat blue panels. A professional classroom handout. No wiring clutter, no decorative shadows, no relay or unrelated devices. Title: "LAB 07 • DHT11 / OLED PIN GUIDE".

Top half: three well-separated illustrated panels.
Panel A heading "DHT11 • BARE SENSOR • FRONT VIEW". Show the blue vented front face of the bare DHT11 and EXACTLY FOUR straight silver legs pointing down. Number legs left to right 1,2,3,4; give each its own non-crossing leader: "1 VCC → 3V3", "2 DATA → GPIO4", "3 NC → leave open", "4 GND → GND". A tiny accurate schematic under it has VCC and DATA as separate rails with a resistor labeled "10 kΩ pull-up" BETWEEN them; no connection to GND through that resistor. Say "Front grille faces you".
Panel B heading "DHT11 • 3-PIN BREAKOUT". Show a blue DHT11 on a small PCB with three connector pads labeled individually "VCC", "DATA / S", "GND". Clearly mark "Example labels — order varies". Add "Check onboard pull-up". This is an illustrative labeled breakout and must not imply every 3-pin PCB has that physical pin order. No bare-sensor pin numbers 1-4 in this panel.
Panel C heading "SSD1306 • I2C • 128x64". Show an OLED module with black blank display, exactly FOUR header pins and explicit printed labels left to right "GND", "VCC", "SCL", "SDA". Four separate leader lines connect each label to its matching pin. State "Example order — read your module labels" and "0x3C in lab code; verify with I2C scan". Do not draw seven-pin SPI OLED or a reset wire.

Bottom half: large, concise mapping table, exactly these columns and rows with no missing entries:
Signal | ESP32-WROOM-32 | ESP32-S3
DHT11 DATA | GPIO4 | GPIO4
OLED SDA | GPIO21 | GPIO8
OLED SCL | GPIO22 | GPIO9
Module VCC | 3V3 | 3V3
Module GND | GND | GND

Use green accent for DATA, blue for SDA, purple for SCL, orange for 3V3, black for GND. Footer: "Use 3.3V-compatible modules • GPIO number is not the header position". Tiny credit "AI-generated teaching illustration". All text must be legible and every leader must end on the correct pin. No claimed live sensor values, no invented physical ESP32 board pin order.
```

แหล่งตรวจ: [Adafruit DHT wiring](https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor), [config.h](../../labs/lab07-smart-home-project/code/lab07_smart_home/config.h)

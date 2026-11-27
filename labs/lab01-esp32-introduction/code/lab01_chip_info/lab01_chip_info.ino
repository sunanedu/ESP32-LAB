/*
 * Lab 01.1 : อ่านข้อมูลบอร์ด ESP32
 * คำอธิบาย : แสดงรุ่นชิป revision จำนวน core ความถี่ CPU ขนาด Flash
 *             MAC address ของ Wi-Fi station และ heap ที่ยังว่าง
 * วงจร     : ESP32 DevKit V1 ต่อคอมพิวเตอร์ด้วยสาย USB data
 * ไลบรารี  : ไม่ใช้ไลบรารีภายนอก (ใช้ Arduino-ESP32 core 3.x)
 * Board    : ESP32 Dev Module (core 3.x)
 */

#include <Arduino.h>
#include <esp_mac.h>

// อ่านและพิมพ์ MAC address ของ Wi-Fi station โดยไม่ต้องเชื่อมเครือข่าย
void printMacAddress() {
  uint8_t baseMac[6] = {0};
  const esp_err_t result = esp_read_mac(baseMac, ESP_MAC_WIFI_STA);

  if (result != ESP_OK) {
    Serial.println("[CHIP] อ่าน MAC address ไม่สำเร็จ");
    return;
  }

  Serial.printf(
      "[CHIP] Wi-Fi STA MAC    : %02X:%02X:%02X:%02X:%02X:%02X\n",
      baseMac[0], baseMac[1], baseMac[2],
      baseMac[3], baseMac[4], baseMac[5]);
}

void setup() {
  // เริ่ม Serial แล้วเผื่อเวลาเปิด Monitor 1 วินาที (ไม่ได้ตรวจว่า Monitor พร้อม)
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("=== ESP32 Chip Information ===");

  // อ่านข้อมูลจาก Arduino-ESP32 core และพิมพ์ด้วยหน่วยที่เข้าใจง่าย
  Serial.printf("[CHIP] Model            : %s\n", ESP.getChipModel());
  Serial.printf("[CHIP] Revision         : %u\n",
                static_cast<unsigned int>(ESP.getChipRevision()));
  Serial.printf("[CHIP] CPU cores        : %u\n",
                static_cast<unsigned int>(ESP.getChipCores()));
  Serial.printf("[CHIP] CPU frequency    : %lu MHz\n",
                static_cast<unsigned long>(ESP.getCpuFreqMHz()));
  Serial.printf("[CHIP] Flash size       : %lu MB\n",
                static_cast<unsigned long>(
                    ESP.getFlashChipSize() / (1024UL * 1024UL)));
  printMacAddress();
  Serial.printf("[CHIP] Free heap        : %lu bytes\n",
                static_cast<unsigned long>(ESP.getFreeHeap()));

  Serial.println("=== Ready ===");
}

void loop() {
  // แล็บนี้อ่านข้อมูลเพียงครั้งเดียว จึงไม่มีงานที่ต้องทำซ้ำ
  delay(1000);
}

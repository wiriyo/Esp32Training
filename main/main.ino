/*
 * ESP32 DevKit - LED Blink Web Dashboard
 *
 * โครงสร้างไฟล์:
 *   main.ino       — WiFi setup, setup(), loop()
 *   blink.ino      — LED blink logic
 *   dashboard.ino  — HTTP Web Dashboard
 *   html_content.h — HTML template
 *
 * วิธีใช้:
 *  1. แก้ไข WIFI_SSID และ WIFI_PASSWORD
 *  2. Upload → เปิด Serial Monitor (115200 baud) ดู IP
 *  3. เปิด browser ไปที่ http://<IP>
 */

#include <WiFi.h>
#include <WebServer.h>

// ===== ตั้งค่า WiFi =====
const char* WIFI_SSID     = "YOUR_SSID";      // ชื่อ WiFi
const char* WIFI_PASSWORD = "YOUR_PASSWORD";  // รหัส WiFi

void setup() {
  Serial.begin(115200);

  // เชื่อมต่อ WiFi
  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  // เริ่มต้น module
  blinkSetup();
  dashboardSetup();
}

void loop() {
  blinkLoop();
  dashboardLoop();
}

/*
 * ESP32 DevKit - LED Blink Web Dashboard
 *
 * แสดงสถานะและควบคุมไฟ LED ผ่าน HTTP Dashboard
 * Built-in LED อยู่ที่ขา GPIO 2
 *
 * Board  : ESP32 Dev Module
 * Library: WiFi, WebServer (built-in ESP32)
 *
 * วิธีใช้:
 *  1. แก้ไข WIFI_SSID และ WIFI_PASSWORD ให้ตรงกับ WiFi ของคุณ
 *  2. Upload แล้วเปิด Serial Monitor (115200 baud)
 *  3. ดู IP Address แล้วเปิด browser ไปที่ http://<IP>
 *
 * โครงสร้างไฟล์:
 *  led_blink_web.ino  - โค้ดหลัก (WiFi, WebServer, logic)
 *  html_content.h     - HTML template ของ Dashboard
 */

#include <WiFi.h>
#include <WebServer.h>
#include "html_content.h"

// ===== ตั้งค่า WiFi =====
const char* WIFI_SSID     = "YOUR_SSID";      // ชื่อ WiFi
const char* WIFI_PASSWORD = "YOUR_PASSWORD";  // รหัส WiFi

// ===== ตั้งค่า LED =====
#define LED_PIN       2      // GPIO 2 = Built-in LED
#define DEFAULT_DELAY 1000   // หน่วง ms (default)

WebServer server(80);

bool          blinking   = true;
int           blinkDelay = DEFAULT_DELAY;
bool          ledState   = false;
unsigned long lastBlink  = 0;

// ===== สร้างหน้า HTML โดยแทนค่า placeholder =====
String buildPage() {
  String page = String(HTML_TEMPLATE);

  page.replace("%LED_COLOR%",    ledState   ? "#00e676" : "#555555");
  page.replace("%BTN_COLOR%",    blinking   ? "#e53935" : "#43a047");
  page.replace("%BLINK_STATUS%", blinking   ? "กระพริบ" : "หยุด");
  page.replace("%LED_STATE%",    ledState   ? "ON"      : "OFF");
  page.replace("%BTN_TEXT%",     blinking   ? "หยุดกระพริบ" : "เริ่มกระพริบ");
  page.replace("%DELAY_VAL%",    String(blinkDelay));
  page.replace("%ESP_IP%",       WiFi.localIP().toString());

  return page;
}

// ===== Route Handlers =====
void handleRoot() {
  server.send(200, "text/html; charset=utf-8", buildPage());
}

void handleToggle() {
  blinking = !blinking;
  if (!blinking) digitalWrite(LED_PIN, LOW);
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleSpeed() {
  if (server.hasArg("delay")) {
    blinkDelay = constrain(server.arg("delay").toInt(), 100, 3000);
  }
  server.send(200, "text/plain", "OK");
}

// ===== Setup =====
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  Serial.print("Connecting to WiFi");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
  Serial.println("Open browser: http://" + WiFi.localIP().toString());

  server.on("/",       handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/speed",  handleSpeed);
  server.begin();
}

// ===== Loop =====
void loop() {
  server.handleClient();

  if (blinking && (millis() - lastBlink >= (unsigned long)blinkDelay)) {
    lastBlink = millis();
    ledState  = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}

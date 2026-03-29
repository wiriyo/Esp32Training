// =============================================================
//  dashboard.ino — HTTP Web Dashboard
// =============================================================

#include "html_content.h"

WebServer server(80);

// สร้างหน้า HTML โดยแทนค่า placeholder ด้วยสถานะจริง
String buildPage() {
  String page = String(HTML_TEMPLATE);

  page.replace("%LED_COLOR%",    ledState  ? "#00e676" : "#555555");
  page.replace("%BTN_COLOR%",    blinking  ? "#e53935" : "#43a047");
  page.replace("%BLINK_STATUS%", blinking  ? "กระพริบ" : "หยุด");
  page.replace("%LED_STATE%",    ledState  ? "ON"      : "OFF");
  page.replace("%BTN_TEXT%",     blinking  ? "หยุดกระพริบ" : "เริ่มกระพริบ");
  page.replace("%DELAY_VAL%",    String(blinkDelay));
  page.replace("%ESP_IP%",       WiFi.localIP().toString());

  return page;
}

void handleRoot() {
  server.send(200, "text/html; charset=utf-8", buildPage());
}

void handleToggle() {
  blinkToggle();
  server.sendHeader("Location", "/");
  server.send(302, "text/plain", "");
}

void handleSpeed() {
  if (server.hasArg("delay")) {
    blinkSetSpeed(server.arg("delay").toInt());
  }
  server.send(200, "text/plain", "OK");
}

void dashboardSetup() {
  server.on("/",       handleRoot);
  server.on("/toggle", handleToggle);
  server.on("/speed",  handleSpeed);
  server.begin();
  Serial.println("Dashboard ready: http://" + WiFi.localIP().toString());
}

void dashboardLoop() {
  server.handleClient();
}

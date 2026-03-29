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
 */

#include <WiFi.h>
#include <WebServer.h>

// ===== ตั้งค่า WiFi =====
const char* WIFI_SSID     = "YOUR_SSID";      // ชื่อ WiFi
const char* WIFI_PASSWORD = "YOUR_PASSWORD";  // รหัส WiFi

// ===== ตั้งค่า LED =====
#define LED_PIN       2      // GPIO 2 = Built-in LED
#define DEFAULT_DELAY 1000   // หน่วง ms (default)

WebServer server(80);

bool  blinking   = true;   // สถานะกระพริบ
int   blinkDelay = DEFAULT_DELAY;
bool  ledState   = false;
unsigned long lastBlink = 0;

// ===== HTML Dashboard =====
String buildPage() {
  String color  = ledState ? "#00e676" : "#555";
  String btnTxt = blinking ? "หยุดกระพริบ" : "เริ่มกระพริบ";
  String btnCol = blinking ? "#e53935" : "#43a047";

  String html = R"rawhtml(<!DOCTYPE html>
<html lang="th">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ESP32 LED Dashboard</title>
  <style>
    body { font-family: sans-serif; background:#1e1e2e; color:#cdd6f4;
           display:flex; flex-direction:column; align-items:center;
           justify-content:center; min-height:100vh; margin:0; }
    h1   { font-size:1.6rem; margin-bottom:0.3rem; }
    .card { background:#313244; border-radius:16px; padding:2rem 3rem;
            text-align:center; box-shadow:0 4px 20px #0006; }
    .led { width:80px; height:80px; border-radius:50%; margin:1rem auto;
           background:)rawhtml" + color + R"rawhtml(;
           box-shadow:0 0 20px )rawhtml" + color + R"rawhtml(; transition:background .3s; }
    .status { font-size:1.1rem; margin-bottom:1.5rem; }
    .btn { display:inline-block; padding:.7rem 1.6rem; border-radius:8px;
           color:#fff; font-size:1rem; text-decoration:none; margin:.4rem;
           cursor:pointer; border:none; }
    .btn-toggle { background:)rawhtml" + btnCol + R"rawhtml(; }
    .speed-box { margin-top:1.2rem; }
    label { font-size:.9rem; }
    input[type=range] { width:200px; margin:.5rem 0; }
    .delay-val { font-weight:bold; color:#89b4fa; }
    .info { margin-top:1.5rem; font-size:.8rem; color:#888; }
  </style>
  <script>
    setTimeout(() => location.reload(), 2000);
    function setSpeed(v){
      document.getElementById('dv').innerText = v + ' ms';
      fetch('/speed?delay=' + v);
    }
  </script>
</head>
<body>
<div class="card">
  <h1>ESP32 LED Dashboard</h1>
  <div class="led"></div>
  <div class="status">สถานะ: <b>)rawhtml" + String(blinking ? "กระพริบ" : "หยุด") + R"rawhtml(</b> |
    LED: <b>)rawhtml" + String(ledState ? "ON" : "OFF") + R"rawhtml(</b></div>
  <a class="btn btn-toggle" href="/toggle">)rawhtml" + btnTxt + R"rawhtml(</a>
  <div class="speed-box">
    <label>ความเร็ว: <span id="dv" class="delay-val">)rawhtml" + String(blinkDelay) + R"rawhtml( ms</span></label><br>
    <input type="range" min="100" max="3000" step="100"
           value=")rawhtml" + String(blinkDelay) + R"rawhtml("
           onchange="setSpeed(this.value)" oninput="document.getElementById('dv').innerText=this.value+' ms'">
    <br><small>100 ms = เร็ว &nbsp;|&nbsp; 3000 ms = ช้า</small>
  </div>
  <div class="info">ESP32 IP: )rawhtml" + WiFi.localIP().toString() + R"rawhtml(</div>
</div>
</body></html>)rawhtml";

  return html;
}

// ===== Route Handlers =====
void handleRoot() {
  server.send(200, "text/html", buildPage());
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

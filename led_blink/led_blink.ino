/*
 * ESP32 DevKit - LED Blink Test
 *
 * ทดสอบไฟ LED ในตัว (Built-in LED) บน ESP32 DevKit
 * Built-in LED อยู่ที่ขา GPIO 2
 *
 * Board: ESP32 Dev Module
 */

#define LED_BUILTIN_PIN 2   // ขา GPIO 2 สำหรับ Built-in LED บน ESP32 DevKit

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN_PIN, OUTPUT);
  Serial.println("ESP32 LED Blink Test Started");
}

void loop() {
  digitalWrite(LED_BUILTIN_PIN, HIGH);   // เปิด LED
  Serial.println("LED ON");
  delay(1000);                           // รอ 1 วินาที

  digitalWrite(LED_BUILTIN_PIN, LOW);    // ปิด LED
  Serial.println("LED OFF");
  delay(1000);                           // รอ 1 วินาที
}

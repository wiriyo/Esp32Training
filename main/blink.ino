// =============================================================
//  blink.ino — LED blink logic
// =============================================================

#define LED_PIN       2       // GPIO 2 = Built-in LED
#define DEFAULT_DELAY 1000    // หน่วง ms เริ่มต้น

bool          blinking   = true;
int           blinkDelay = DEFAULT_DELAY;
bool          ledState   = false;
unsigned long lastBlink  = 0;

void blinkSetup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void blinkLoop() {
  if (!blinking) return;
  if (millis() - lastBlink >= (unsigned long)blinkDelay) {
    lastBlink = millis();
    ledState  = !ledState;
    digitalWrite(LED_PIN, ledState);
  }
}

void blinkSetSpeed(int ms) {
  blinkDelay = constrain(ms, 100, 3000);
}

void blinkToggle() {
  blinking = !blinking;
  if (!blinking) {
    ledState = false;
    digitalWrite(LED_PIN, LOW);
  }
}

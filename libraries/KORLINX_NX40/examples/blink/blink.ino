#include <Adafruit_TinyUSB.h>  // keeps the USB port, which the IDE resets to upload

// Blinks the built-in LED, the red one, twice a second

bool on;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  on = !on;
  digitalWrite(LED_BUILTIN, on);
  delay(500);
}

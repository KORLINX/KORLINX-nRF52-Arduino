#include <Adafruit_TinyUSB.h>  // keeps the USB port, which the IDE resets to upload

// Fades the red LED in and out

int brightness = 0;
int step = 5;

void setup() {
}

void loop() {
  analogWrite(LED_RED, brightness);
  brightness += step;
  if (brightness <= 0 || brightness >= 255) {
    step = -step;
  }
  delay(30);
}

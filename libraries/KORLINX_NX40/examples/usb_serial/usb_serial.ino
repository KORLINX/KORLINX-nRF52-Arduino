#include <Adafruit_TinyUSB.h>  // for Serial

// Prints the core version once, then the uptime in milliseconds every second

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.printf("NX40 up, BSP %s, %.2f\n", ARDUINO_BSP_VERSION, 3.14f);
}

void loop() {
  Serial.println(millis());
  delay(1000);
}

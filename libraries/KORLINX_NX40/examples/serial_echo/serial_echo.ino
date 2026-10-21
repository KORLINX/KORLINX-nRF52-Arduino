#include <Adafruit_TinyUSB.h>  // for Serial

// Reads lines from Serial, the NX40's own USB port, and sends them back.
// Set the Serial Monitor to send a line ending, or no line ever ends.

String line;

void setup() {
  Serial.begin(115200);
  while (!Serial) delay(10);  // the NX40's USB serial port opens with the Serial Monitor
  Serial.println("Type a line and press Enter");
}

void loop() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c != '\r' && c != '\n') {
      line += c;
    } else if (line.length() > 0) {
      Serial.print("You typed: ");
      Serial.println(line);
      line = "";
    }
  }
}

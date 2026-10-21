#include <Adafruit_TinyUSB.h>  // for Serial

// Counts presses of SW2 and lights the blue LED while it is held. The
// button is active low.

int lastState = HIGH;
unsigned long lastChange;
int presses;

void setup() {
  pinMode(PIN_BUTTON1, INPUT_PULLUP);
  pinMode(LED_BLUE, OUTPUT);
  digitalWrite(LED_BLUE, !LED_STATE_ON);
  Serial.begin(115200);
  while (!Serial) delay(10);  // the NX40's USB serial port opens with the Serial Monitor
  Serial.println("Press the button");
}

void loop() {
  int state = digitalRead(PIN_BUTTON1);
  // after a change, ignore the contact bounce for 20 ms
  if (state != lastState && millis() - lastChange >= 20) {
    lastState = state;
    lastChange = millis();
    digitalWrite(LED_BLUE, state == LOW ? LED_STATE_ON : !LED_STATE_ON);
    if (state == LOW) {
      Serial.print("Pressed ");
      Serial.println(++presses);
    }
  }
}

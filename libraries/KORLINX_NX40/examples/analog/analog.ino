#include <Adafruit_TinyUSB.h>  // for Serial

// Prints the voltage on A0-A6 of the J3 header. analogRead() reads 0-3.6 V
// in 10 bits, with gain 1/6 on the 0.6 V internal reference.

const int inputs[] = {A0, A1, A2, A3, A4, A5, A6};

void setup() {
  Serial.begin(115200);
}

void loop() {
  for (size_t i = 0; i < sizeof(inputs) / sizeof(inputs[0]); i++) {
    Serial.print('A');
    Serial.print(i);
    Serial.print(' ');
    Serial.print(analogRead(inputs[i]) * 3600L / 1024);
    Serial.print(" mV  ");
  }
  Serial.println();
  delay(500);
}

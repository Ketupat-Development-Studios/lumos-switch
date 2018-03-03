#include<SoftwareSerial.h>
SoftwareSerial BTserial(3, 2);  //RX pin | TX pin (relative to the Arduino)

char c = ' ';

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino is ready");
  BTserial.begin(38400);
}

void loop() {
  //Keep reading from HC-05 and send to arduino serial monitor
  if (BTserial.available()) {
    c = BTserial.read();
    Serial.write(c);
  }

  //Keep reading from arduino serial monitor and send to HC-05
  if (Serial.available()) {
    c = Serial.read();
    BTserial.write(c);
  }
}


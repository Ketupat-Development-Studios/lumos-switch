#include<SoftwareSerial.h>
#include<Servo.h>
SoftwareSerial BTserial(3, 2);
Servo actuator;

char c = ' ';
char blueIn = ' ';
boolean readingData = false;
boolean open_state = true;

//int upPin = 4;
//int downPin = 2;
int upVal, downVal;
int upArm = 0;
int downArm = 120;
int restArm = 50;
int state = 999;
int moving = 0;
int move_delay = 150;
int indicator = 0;
int indicator_delay = 100;
int touch_switch = 0;
int touch_switch_delay = 50;
int pairing = 0;
int pairing_delay = 50;

//PINS declaration INPUT
int blueRX = 3;

//PINS declaration OUTPUT
int blueTX = 2;
int blueEN = 5;
int servoCtl = 8;
int touchSensor = 10;
int ind2 = 11;
int ind1 = 12;

void setup() {
  Serial.begin(9600);
  Serial.println("Arduino is ready");
  BTserial.begin(38400);

  pinMode(blueRX, INPUT);
  pinMode(blueTX, OUTPUT);
  pinMode(blueEN, OUTPUT);
  pinMode(touchSensor, OUTPUT);
  pinMode(ind2, OUTPUT);
  pinMode(ind1, OUTPUT);

  pinMode(9, INPUT_PULLUP);
  
  actuator.attach(servoCtl);

  //Base States
  digitalWrite(ind1, LOW);
  digitalWrite(ind2, HIGH);
  digitalWrite(touchSensor, HIGH);
  digitalWrite(blueTX, LOW);
  digitalWrite(blueEN, LOW);
}

void loop() {
  if (readingData && indicator <= 0) {
    indicator = 0;
    readingData = false;
  }
  else if (indicator > 0) {
    indicator--;
  }
  else {
    readingData = false;
  }
  //Keep reading from HC-05 and send to arduino serial monitor
  if (BTserial.available()) {
    c = BTserial.read();
    blueIn = c;
    Serial.write(c);
    readingData = true;
    indicator = indicator_delay;
  }

  //Keep reading from arduino serial monitor and send to HC-05
  if (Serial.available()) {
    c = Serial.read();
    BTserial.write(c);
  }

  upVal = HIGH;
  downVal = HIGH;
  int tmp = digitalRead(9);
//  Serial.write("GATE: ");
//  Serial.write(tmp==LOW?"LOW":"HIGH");
//  Serial.write("\n");
  if (touch_switch <= 0) {
    if (tmp==LOW) {
      if (open_state == true) {
        BTserial.write("C\n");
        BTserial.flush();
        Serial.write("Circuit Closed\n");
        touch_switch = touch_switch_delay;
      }
      open_state = false;
    }
    else {
      if (open_state == false) {
        BTserial.write("O\n");
        BTserial.flush();
        Serial.write("Circuit Open\n");
        touch_switch = touch_switch_delay;
      }
      open_state = true;
    }
  }
  else {
    touch_switch--;
    if (touch_switch%25 == 0) {
      Serial.write("--> Cooldown: ");
      Serial.print(touch_switch);
      Serial.write("\n");
    }
  }

  if (pairing > 0) pairing--;
  
  if (readingData) digitalWrite(ind1, HIGH);
  else digitalWrite(ind1, LOW);

  //Hook on bluetooth control --> U (up) D (down)
  if (blueIn == 'U') upVal = LOW;
  else if (blueIn == 'D') downVal = LOW;
  else if (blueIn == 'T') {
    if (pairing <= 0) {
      digitalWrite(ind1, LOW);
      digitalWrite(ind2, LOW);
      delay(120);
      for (int i=0;i<5;i++) {
        digitalWrite(ind2, HIGH);
        delay(120);
        digitalWrite(ind2, LOW);
        delay(120);
      }
      for (int i=0;i<3;i++) {
        digitalWrite(ind1, HIGH);
        delay(250);
        digitalWrite(ind1, LOW);
        delay(250);
      }
      delay(1000);
      pairing = pairing_delay;
    }
  }
  
//  Serial.write("UP: ");
//  Serial.write(upVal==LOW?"LOW":"HIGH");
//  Serial.write(" | DOWN: ");
//  Serial.write(downVal==LOW?"LOW":"HIGH");
//  Serial.write("\n");
  if (moving > 0) {
    moving--;
    digitalWrite(ind2, LOW);
  }
  else {
    if (upVal == HIGH && downVal == HIGH) {
      //Resting
      if (state != 0) {
        actuator.write(restArm);
        state = 0;
        moving = move_delay;
      }
      else digitalWrite(ind2, HIGH);
    }
    else {
      if (downVal == LOW) {
        if (state != -1) {
          actuator.write(downArm);
          Serial.write("DOWN\n");
          state = -1;
          moving = move_delay;
        }
        else digitalWrite(ind2, HIGH);
      }
      else if (upVal == LOW) {
        if (state != 1) {
          actuator.write(upArm);
          Serial.write("UP\n");
          state = 1;
          moving = move_delay;
        }
        else digitalWrite(ind2, HIGH);
      }
    }
  }
  delay(5);
}


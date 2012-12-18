/*
 Test Servo Program
 
 Copyright @ InfiniteSense.com
 */ 
#include <Servo.h> 

#define SERVO_PIN 9

// create servo object to control a servo 
// a maximum of eight servo objects can be created 
Servo myservo;
int pos = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  initServo();
}

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {
    pos = Serial.parseInt();
    if (Serial.read() == '\n') {
      // say what you got:
      Serial.print("I received: ");
      Serial.println(pos, DEC);
      driveServo();
    }
  }
}

void initServo() {
  myservo.attach(SERVO_PIN);  // attaches the servo on pin 9 to the servo object 
}

void driveServo() {
  myservo.write(pos);
  delay(15);
}

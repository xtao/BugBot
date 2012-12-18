/*
 Test Motor Program
 
 Copyright @ InfiniteSense.com
 */

#define MOTOR_PIN 5

int incomingByte = 0;
void setup() {
  Serial.begin(9600);
  initMotor();
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    incomingByte = Serial.parseInt();
    if (Serial.read() == '\n') {
      // say what you got:
      Serial.print("I received: ");
      Serial.println(incomingByte, DEC);
      driveMotor();
    }
  }
}

void initMotor() {
  pinMode(MOTOR_PIN, OUTPUT); 
}

void driveMotor() {
  analogWrite(MOTOR_PIN, incomingByte);
}

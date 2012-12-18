/*
 Test IR Program
 
 Copyright @ InfiniteSense.com
 */

#define IR_INPUT_PIN A0
#define IR_CONTROL_PIN 4


void setup()
{
  Serial.begin(9600);
  initIR();
}

void loop()
{
  // ...
}

void initIR() {
  pinMode(IR_INPUT_PIN, INPUT);  
  pinMode(IR_CONTROL_PIN, OUTPUT);
  digitalWrite(IR_CONTROL_PIN, HIGH);
}

void readIR() {

}

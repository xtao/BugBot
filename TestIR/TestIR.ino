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
  readIR();
}

void initIR() {
  pinMode(IR_INPUT_PIN, INPUT);  
  pinMode(IR_CONTROL_PIN, OUTPUT);
  digitalWrite(IR_CONTROL_PIN, HIGH);
}

void readIR() {
  int ir_diff;
  ir_diff = analogRead(IR_INPUT_PIN);
  Serial.print(ir_diff);
  digitalWrite(IR_CONTROL_PIN, LOW);
  delay(500);
  Serial.print(" ");
  ir_diff = analogRead(IR_INPUT_PIN) - ir_diff;
  Serial.print(ir_diff);
  digitalWrite(IR_CONTROL_PIN, HIGH);
  Serial.print("\n");
}

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
  delay(500);
}

void initIR() {
  pinMode(IR_INPUT_PIN, INPUT);  
  pinMode(IR_CONTROL_PIN, OUTPUT);
  digitalWrite(IR_CONTROL_PIN, HIGH);
}

void readIR() {
  int ir_diff;
  int ir_temp;
  ir_diff = analogRead(IR_INPUT_PIN);
  Serial.print(ir_diff);
  digitalWrite(IR_CONTROL_PIN, LOW);
  delay(500);
  Serial.print(" ");
  ir_temp = analogRead(IR_INPUT_PIN);
  Serial.print(ir_temp);
  Serial.print(" ");
  ir_diff = ir_temp - ir_diff;
  Serial.print(ir_diff);
  digitalWrite(IR_CONTROL_PIN, HIGH);
  Serial.print("\n");
}

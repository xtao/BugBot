/*
 Test IR Program
 
 Copyright @ InfiniteSense.com
 */

/* ir pin */
#define PIN_IR1_LEFT A0
#define PIN_IR2_LEFT A1
#define PIN_IR3_LEFT A2
#define PIN_IR3_RIGHT A3
#define PIN_IR2_RIGHT A4
#define PIN_IR1_RIGHT A5

#define IR_NUM 6

int ir_array[IR_NUM] = { 0 };
const int ir_pin_array[IR_NUM] = {
   PIN_IR1_LEFT, PIN_IR2_LEFT, PIN_IR3_LEFT, PIN_IR3_RIGHT, PIN_IR2_RIGHT, PIN_IR1_RIGHT
};

void setup()
{
  Serial.begin(9600);
  initIR();
}

void loop()
{
  readIR();
  delay(1000);
}

void initIR() {
  int i;
  // setting input pins
  for (i = 0; i < IR_NUM; i++) {
    pinMode(ir_pin_array[i], INPUT);
  }
}

void readIR() {
  int i;
  for (i = 0; i < IR_NUM; i++) {
     ir_array[i] = digitalRead(ir_pin_array[i]);
     if (ir_array[i] == LOW) {
       Serial.print("0 ");
     } else {
       Serial.print("1 ");
     }
  }
  Serial.print("\n");
}

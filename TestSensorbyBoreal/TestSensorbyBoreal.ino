int left1 = A0;
int left2 = A1;
int left3 = A2;
int right3 = A3;
int right2 = A4;
int right1 = A5;

int ir_pin = 3;
int ir_en_pin = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(left1, INPUT);
  pinMode(left2, INPUT);
  pinMode(left3, INPUT);
  pinMode(right3, INPUT);
  pinMode(right2, INPUT);
  pinMode(right1, INPUT);
  
  pinMode(ir_pin, INPUT);
}

void loop() 
{ 
  int num1,num2,num3,num4,num5,num6,num7;
  num1=digitalRead(left1);   
  num2=digitalRead(left2);
  num3=digitalRead(left3);
  num4=digitalRead(right3);
  num5=digitalRead(right2);
  num6=digitalRead(right1);
  num7=digitalRead(ir_pin);
  
  if (num7 == LOW) {
    Serial.println("Obstruction");
    delayMicroseconds(2);
  }
  
  if(num1==0)          // Left1
  {
    Serial.println("turnL200");   
   delayMicroseconds(2);
  }
 else if(num2==0)       // Left2
 {
   Serial.println("turnL150");
   delayMicroseconds(2);
 }
  else if(num3==0)       //Left3
 {
   Serial.println("turnL100");
   delayMicroseconds(2);
 } 
 else if(num4==0)       //Right3
 {
    Serial.println("turnR100");
   delayMicroseconds(2);
 }
else if(num5==0)       //Right2
 {
    Serial.println("turnR150");
   delayMicroseconds(2);
 }
 
 else if(num6==0)        //Right1
 {
    Serial.println("turnR200");
   delayMicroseconds(2);
 }
  else                     //其他状态小车直走
  {
    Serial.println("trunfoward180");
   delayMicroseconds(1000);
  }
  
}

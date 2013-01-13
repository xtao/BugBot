int left1 = A0;
int left2 = 1;
int left3 = 2;
int right3 = 3;
int right2 = 4;
int right1 = 5;

void setup()
{
  Serial.begin(9600);

  
  pinMode(left1, INPUT);
  pinMode(left2, INPUT);
  pinMode(left3, INPUT);
  pinMode(right3, INPUT);
  pinMode(right2, INPUT);
  pinMode(right1, INPUT);
  
}

void loop() 
{ 
  int num1,num2,num3,num4,num5,num6;
  num1=digitalRead(left1);   
  num2=digitalRead(left2);
  num3=digitalRead(left3);
  num4=digitalRead(right3);
  num5=digitalRead(right2);
  num6=digitalRead(right1);
  
  
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

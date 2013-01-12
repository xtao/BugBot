// pwm port 3,5,6, 9, 10, 11
const int pwm_l = 11;
const int dir1_l = 10;
const int dir2_l = 9;
const int pwm_r = 6;
const int dir1_r = 5;
const int dir2_r = 4;
// sensor
const int left1 = A0;
const int left2 = A1;
const int left3 = A2;
const int right3 = A3;
const int right2 = A4;
const int right1 = A5;

void motor(char dirpin1,char dirpin2, char pwmpin,char state,int val)
{
  if (val == 0){
    analogWrite(pwmpin,val);
    digitalWrite(dirpin1,HIGH);
    digitalWrite(dirpin2,HIGH);  
    return;  
  }
 
 if(state==1)
 {  
    analogWrite(pwmpin,val);
    digitalWrite(dirpin1,HIGH);
    digitalWrite(dirpin2,LOW);  
 }
 else if(state==0)
 {
    analogWrite(pwmpin,val);
    digitalWrite(dirpin1,LOW);
    digitalWrite(dirpin2,HIGH); 
 }
}
 
void runforward(int i)   //前进
{
  motor(dir1_r,dir2_r, pwm_r, 1, i);
  motor(dir1_l,dir2_l, pwm_l, 1, i);
}
void runback(int j)    //后退
{
  motor(dir1_r,dir2_r, pwm_r, 0, j);
  motor(dir1_l,dir2_l, pwm_l, 0, j);
}
void turnL(int m)     //左转
{
  motor(dir1_r,dir2_r, pwm_r, 1, m);
  motor(dir1_l,dir2_l, pwm_l, 0, m);
}
void turnR(int n)      //右转 
{
  motor(dir1_r,dir2_r, pwm_r, 0, n);
  motor(dir1_l,dir2_l, pwm_l, 1, n);
}
void stop()            //停止
{
  motor(dir1_r,dir2_r, pwm_r, 0, 0);
  motor(dir1_l,dir2_l, pwm_l, 1, 0); 
}

void setup()
{
   Serial.begin(9600);
   pinMode(dir1_l, OUTPUT);   
   pinMode(dir1_r, OUTPUT);   
   pinMode(dir2_l, OUTPUT);   
   pinMode(dir2_r, OUTPUT);   

   pinMode(left1, INPUT);
   pinMode(left2, INPUT);
   pinMode(left3, INPUT);
   pinMode(right3, INPUT);
   pinMode(right2, INPUT);
   pinMode(right1, INPUT);
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
  
  Serial.println(num6);
  delayMicroseconds(20);
  
  if(num1==LOW)          // Left1
  {
    turnL(150);
    //Serial.println("turnL200");   
   delayMicroseconds(2);
  }
  else if(num2==LOW)       // Left2
  {
   turnL(120);
   //Serial.println("turnL150");
   delayMicroseconds(2);
  }
  /*
  else if(num3==0)       //Left3
  {
   turnL(100);
   //Serial.println("turnL100");
   delayMicroseconds(2);
  } 
  else if(num4==0)       //Right3
  {
   turnR(150);
    //Serial.println("turnR100");
   delayMicroseconds(2);
  }*/
  else if(num5==LOW)       //Right2
  {
   turnR(120);
   //Serial.println("turnR150");
   delayMicroseconds(2);
  }
  else if(num6==LOW)        //Right1
  {
   turnR(150);
   //Serial.println("turnR200");
   delayMicroseconds(2);
  }
  else                     //其他状态小车直走
  {
    runforward(150);
   //Serial.println("trunfoward180");
   delay(2);
  }
}

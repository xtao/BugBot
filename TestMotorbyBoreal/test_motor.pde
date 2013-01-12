// pwm port 3,5,6, 9, 10, 11
const int pwm_l = 11;
const int dir1_l = 10;
const int dir2_l = 9;
const int pwm_r = 6;
const int dir1_r = 5;
const int dir2_r = 4;

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
   pinMode(dir1_l, OUTPUT);   
   pinMode(dir1_r, OUTPUT);   
   pinMode(dir2_l, OUTPUT);   
   pinMode(dir2_r, OUTPUT);   
}

void loop()
{
   motor(dir1_r,dir2_r, pwm_r, 1, 100);
   delay(1000);
   motor(dir1_r,dir2_r, pwm_r, 1, 0);
   
   motor(dir1_l,dir2_l, pwm_l, 1, 100);
   delay(1000);
   motor(dir1_l,dir2_l, pwm_l, 1, 0);
   while(1){
   }
   /*
   runforward(180); 
   delay(1000);
   runback(180);
   delay(1000);
   turnL(180);
   delay(1000);
   turnR(180);
   delay(1000);
   */
}


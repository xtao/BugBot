/*
 Test Angle Program
 
 Copyright @ InfiniteSense.com
 */
 
/* pwm port 3, 5, 6, 9, 10, 11 */
#define PIN_PWM_LEFT 11
#define PIN_DIR1_LEFT 10
#define PIN_DIR2_LEFT 9
#define PIN_PWM_RIGHT 6
#define PIN_DIR1_RIGHT 5
#define PIN_DIR2_RIGHT 4

#define MOTOR_NUM 2
#define MOTOR_PWM 0
#define MOTOR_DIR1 1
#define MOTOR_DIR2 2
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

int motor_array[3][MOTOR_NUM] = { 0 };
long motor_delay = 0; // in us
const int motor_pin_array[3][MOTOR_NUM] = {
  {PIN_PWM_LEFT, PIN_PWM_RIGHT}, // digital PWM pins
  {PIN_DIR1_LEFT, PIN_DIR1_RIGHT},
  {PIN_DIR2_LEFT, PIN_DIR2_RIGHT}
};


void setup()
{ 
  // initialize serial:
  Serial.begin(9600);
  initMotor();
}

void loop()
{
  // if there's any serial available, read it:
  while (Serial.available() > 0) {

    // look for the next valid integer in the incoming serial stream:
    int delay = Serial.parseInt(); 

    // look for the newline. That's the end of your
    // sentence:
    if (Serial.read() == '\n') {
      turnLeft(180, delay);
      Serial.println(delay, DEC);
      driveMotor();
      stopMotor();
    }
  }
}

void initMotor()
{
  int i;
  for (i = 0; i < MOTOR_NUM; i++) {
    pinMode(motor_pin_array[MOTOR_DIR1][i], OUTPUT);
    pinMode(motor_pin_array[MOTOR_DIR2][i], OUTPUT);
  }
}

void turnLeft(int speed, int delay)
{
  motorBackward(MOTOR_LEFT, speed);
  motorForward(MOTOR_RIGHT, speed);
  motor_delay = delay;
}

void motorForward(int motor, int speed)
{
  motor_array[MOTOR_PWM][motor] = speed;
  motor_array[MOTOR_DIR1][motor] = HIGH;
  motor_array[MOTOR_DIR2][motor] = LOW;
  //analogWrite(motor_array[MOTOR_PWM][motor], speed);
  //digitalWrite(motor_array[MOTOR_DIR1][motor], HIGH);
  //digitalWrite(motor_array[MOTOR_DIR2][motor], LOW);
}

void motorBackward(int motor, int speed)
{
  motor_array[MOTOR_PWM][motor] = speed;
  motor_array[MOTOR_DIR1][motor] = LOW;
  motor_array[MOTOR_DIR2][motor] = HIGH;
  //analogWrite(motor_array[MOTOR_PWM][motor], speed);
  //digitalWrite(motor_array[MOTOR_DIR1][motor], LOW);
  //digitalWrite(motor_array[MOTOR_DIR2][motor], HIGH); 
}

void motorStop(int motor)
{
  motor_array[MOTOR_PWM][motor] = 0;
  motor_array[MOTOR_DIR1][motor] = HIGH;
  motor_array[MOTOR_DIR2][motor] = HIGH;
  //analogWrite(motor_array[MOTOR_PWM][motor], 0);
  //digitalWrite(motor_array[MOTOR_DIR1][motor],HIGH);
  //digitalWrite(motor_array[MOTOR_DIR2][motor],HIGH);  
}

int driveMotor()
{
  int i;
  for (i = 0; i < MOTOR_NUM; i++) {
    analogWrite(motor_pin_array[MOTOR_PWM][i], motor_array[MOTOR_PWM][i]);
    digitalWrite(motor_pin_array[MOTOR_DIR1][i], motor_array[MOTOR_DIR1][i]);
    digitalWrite(motor_pin_array[MOTOR_DIR2][i], motor_array[MOTOR_DIR2][i]);
  }
  delay(motor_delay);
}

void stopMotor()
{
  int i;
  for (i = 0; i < MOTOR_NUM; i++) {
    analogWrite(motor_pin_array[MOTOR_PWM][i], 0);
  }
}

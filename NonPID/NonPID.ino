/*
 Main Program For CarBot
 Including IR sensor, Collision sensor, Motor, Servo
 
 Copyright @ InfiniteSense.com
*/
#include <Servo.h> 

/* pwm port 3, 5, 6, 9, 10, 11 */
#define PIN_PWM_LEFT 11
#define PIN_DIR1_LEFT 10
#define PIN_DIR2_LEFT 9
#define PIN_PWM_RIGHT 6
#define PIN_DIR1_RIGHT 5
#define PIN_DIR2_RIGHT 4

/* ir pin */
#define PIN_IR1_LEFT A0
#define PIN_IR2_LEFT A1
#define PIN_IR3_LEFT A2
#define PIN_IR3_RIGHT A3
#define PIN_IR2_RIGHT A4
#define PIN_IR1_RIGHT A5

/* collision */
#define PIN_COLLISION_IR 3

/* servo */
#define PIN_SERVO 2

#define IR_NUM 6
#define IR_NUM_MAX 6
#define MOTOR_NUM 2
#define MOTOR_PWM 0
#define MOTOR_DIR1 1
#define MOTOR_DIR2 2
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

#define PID_P_FACTOR 5.9
#define PID_I_FACTOR 0
#define PID_D_FACTOR 0
#define PID_SCALING_FACTOR 128

#define STATE_TO_TOWER 0
#define STATE_TO_HOME 1
#define STATE_POST_BALL 2
#define STATE_FINDING_LINE 3

struct pid_t {
  int p_factor;
  int i_factor;
  int d_factor;
  int sum_error;
  int last_error;
};

struct pid_t pid = {
  PID_P_FACTOR * PID_SCALING_FACTOR,
  PID_I_FACTOR * PID_SCALING_FACTOR,
  PID_D_FACTOR * PID_SCALING_FACTOR,
  0,
  0
};

int collision_ir = 0;
int ir_array[IR_NUM] = { 0 };
const int ir_pin_array[IR_NUM_MAX] = {
   PIN_IR1_LEFT, PIN_IR2_LEFT, PIN_IR3_LEFT, PIN_IR3_RIGHT, PIN_IR2_RIGHT, PIN_IR1_RIGHT
};
const int ir_pos_map[IR_NUM_MAX] = {
    -5, -3, -1, 1, 3, 5
};

int motor_array[3][MOTOR_NUM] = { 0 };
long motor_delay = 0; // in us
const int motor_pin_array[3][MOTOR_NUM] = {
  {PIN_PWM_LEFT, PIN_PWM_RIGHT}, // digital PWM pins
  {PIN_DIR1_LEFT, PIN_DIR1_RIGHT},
  {PIN_DIR2_LEFT, PIN_DIR2_RIGHT}
};

int car_direction = STATE_TO_TOWER;
Servo myservo;

void setup()
{
  initSerial();
  initMotor();
  initIR();
  initServo();
  delay(1000);
}

void loop() 
{ 
  readIR();
  process();
  driveMotor();
}

void initSerial()
{
  Serial.begin(9600);
}

void process()
{
  // get the position
  // process with PID
  // get result
  int i;
  
  if (collision_ir == LOW) {
    if (car_direction == STATE_TO_TOWER) {
      car_direction = STATE_FINDING_LINE;
      stopDead(1000);
      driveMotor();
      delay(1000);
      myservo.write(45);
      delay(1000);
      runLeft(180, 2);
      for (i = 0; i < 6000; i++) {
        driveMotor();
      }
    } else if (car_direction == STATE_TO_HOME) {
      stopDead(1000);
    }
    return;
  }
  
if (car_direction == STATE_TO_TOWER) {
    // ir 5
    if (ir_array[0] == LOW && ir_array[2] == LOW) {
      runLeft(150, 2);
    } else if (ir_array[1] == LOW && ir_array[2] == LOW) {
      runLeft(120, 2);
    } else if (ir_array[2] == LOW) {
      runForward(150, 2);
    } else if (ir_array[1] == LOW) { 
      runLeft(120, 2);
    } else if (ir_array[3] == LOW) {
      runRight(120, 2);
    } else if (ir_array[0] == LOW) {
      runLeft(150 ,2);
    } else if (ir_array[4] == LOW) {
      runRight(150, 2);
    }
  } else if (car_direction == STATE_TO_HOME) {
    // ir 5
    if (ir_array[4] == LOW && ir_array[3] == LOW && ir_array[2] == LOW) {
      runRight(180, 2);
      // specific process
      for (i = 0; i < 6000; i++) {
        driveMotor();
      }
    } else if (ir_array[4] == LOW && ir_array[2] == LOW) {
      runRight(180, 2);
    } else if (ir_array[3] == LOW && ir_array[2] == LOW) {
      runRight(150, 2);
    } else if (ir_array[2] == LOW) {
      runByPosition(0, 2);
    } else if (ir_array[1] == LOW) {
      runByPosition(4, 2);
    } else if (ir_array[3] == LOW) {
      runByPosition(-4, 2);
    } else if (ir_array[0] == LOW) {
      runByPosition(6, 2);
    } else if (ir_array[4] == LOW) {
      runByPosition(-6, 2);
    }
  } else if (car_direction == STATE_FINDING_LINE) {
    if (ir_array[0] == HIGH && ir_array[1] == HIGH && ir_array[2] == HIGH && ir_array[3] == HIGH && ir_array[4] == HIGH) {
      runLeft(120, 2);
    } else {
      car_direction = STATE_TO_HOME;
    }
  }


}


/* IR Sensor */

void initIR()
{
  int i;
  // setting input pins
  for (i = 0; i < IR_NUM_MAX; i++) {
    pinMode(ir_pin_array[i], INPUT);
  }
  
  pinMode(PIN_COLLISION_IR, INPUT);
}

int readIR()
{
  int i;
  for (i = 0; i < IR_NUM; i++) {
     ir_array[i] = digitalRead(ir_pin_array[i]);
  }
  collision_ir = digitalRead(PIN_COLLISION_IR);
}

/* PID */

int processPID(int setPoint, int processValue, struct pid_t *pid)
{
    int error = 0;
    int p_term = 0;
    int i_term = 0;
    int d_term = 0;
    int ret = 0;
    error = setPoint - processValue;
    p_term = pid->p_factor * error;
    //pid->sum_error = pid->sum_error + error;
    //i_term = pid->i_factor * pid->sum_error;
    d_term = pid->d_factor * (pid->last_error - error);
    pid->last_error = error;
    ret = (p_term + i_term + d_term) / PID_SCALING_FACTOR;
    /* TODO: check value range */
    return ret;
}

void findLine(int speed)
{
  runLeft(speed, 1);
}

void initServo()
{
  myservo.attach(PIN_SERVO);
  myservo.write(90);
}

/* MOTOR */

void initMotor()
{
  int i;
  for (i = 0; i < MOTOR_NUM; i++) {
    pinMode(motor_pin_array[MOTOR_DIR1][i], OUTPUT);
    pinMode(motor_pin_array[MOTOR_DIR2][i], OUTPUT); 
  }
}

void runForward(int speed, int delay)
{
  motorForward(MOTOR_LEFT, speed);
  motorForward(MOTOR_RIGHT, speed);
  motor_delay = delay;
}

void runBackward(int speed, int delay)
{
  motorBackward(MOTOR_LEFT, speed);
  motorBackward(MOTOR_RIGHT, speed);
  motor_delay = delay;
}

void runLeft(int speed, int delay)
{
  motorBackward(MOTOR_LEFT, speed);
  motorForward(MOTOR_RIGHT, speed);
  motor_delay = delay;
}

void runRight(int speed, int delay)
{
  motorForward(MOTOR_LEFT, speed);
  motorBackward(MOTOR_RIGHT, speed);
  motor_delay = delay;
}

void stopDead(int delay)
{
  motorStop(MOTOR_LEFT);
  motorStop(MOTOR_RIGHT);
  motor_delay = delay;
}

void runByPosition(int position, int delay)
{
  if (position < -5) {
    motorForward(MOTOR_LEFT, 150);
    motorForward(MOTOR_RIGHT, 0);
  } else if (position >= -5 && position < -1) {
    motorForward(MOTOR_LEFT, 150); 
    motorForward(MOTOR_RIGHT, 30 * position + 180);
  } else if (position >= -1 && position <= 1) {
    motorForward(MOTOR_LEFT, 120);
    motorForward(MOTOR_RIGHT, 120);
  } else if (position > 1 && position <= 5) {
    motorForward(MOTOR_LEFT, -30 * position + 180);
    motorForward(MOTOR_RIGHT, 150);
  } else if (position > 5) {
    motorForward(MOTOR_LEFT, 0);
    motorForward(MOTOR_RIGHT, 150);
  }
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
  delayMicroseconds(motor_delay);
}


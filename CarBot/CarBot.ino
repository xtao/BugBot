/*
 Main Program For CarBot
 Including IR sensor, Collision sensor, Motor, Servo
 
 Copyright @ InfiniteSense.com
*/

/* pwm port 3,5,6, 9, 10, 11 */
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

#define IR_NUM 6
#define MOTOR_NUM 2
#define MOTOR_PWM 0
#define MOTOR_DIR1 1
#define MOTOR_DIR2 2
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

#define PID_P_FACTOR 1
#define PID_I_FACTOR 0
#define PID_D_FACTOR 0

struct pid_t {
  int p_factor;
  int i_factor;
  int d_factor;
  int sum_error;
  int last_error;
};

struct pid_t pid = {
  PID_P_FACTOR,
  PID_I_FACTOR,
  PID_D_FACTOR,
  0,
  0
};

// sensor
const int left1 = PIN_IR1_LEFT;
const int left2 = PIN_IR2_LEFT;
const int left3 = PIN_IR3_LEFT;
const int right3 = PIN_IR3_RIGHT;
const int right2 = PIN_IR2_RIGHT;
const int right1 = PIN_IR1_RIGHT;

int ir_array[IR_NUM] = { 0 };
const int ir_pin_array[IR_NUM] = {
   PIN_IR1_LEFT, PIN_IR2_LEFT, PIN_IR3_LEFT, PIN_IR3_RIGHT, PIN_IR2_RIGHT, PIN_IR1_RIGHT
};
const int ir_pos_map[IR_NUM] = {
    -3, -2, -1, 1, 2, 3
};

int motor_array[3][MOTOR_NUM] = { 0 };
long motor_delay = 0; // in us
const int motor_pin_array[3][MOTOR_NUM] = {
  {PIN_PWM_LEFT, PIN_PWM_RIGHT}, // digital PWM pins
  {PIN_DIR1_LEFT, PIN_DIR1_RIGHT},
  {PIN_DIR2_LEFT, PIN_DIR2_RIGHT}
};

void setup()
{
  initSerial();
  initMotor();
  initIR();
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

int process()
{
  // get the position
  // process with PID
  // get result
  
  int i;
  int ret = 0;
  int ir_active_num = 0;
  int ir_active = -1;
  for (i = 0; i < IR_NUM; i++) {
    if (ir_array[i] == LOW) {
      ir_active_num++;
      ir_active = (ir_active == -1) ? i : ir_active;
    }
  }
  
  if (ir_active_num > 0) {
    ret = processPID(0, ir_pos_map[ir_active], &pid);
    if (ret > 3) {
      runLeft(180, 2);
    } else if (ret <= 3 && ret > 2) {
      runLeft(150, 2);
    } else if (ret <= 2 && ret > 1) {
      runLeft(120, 2);
    } else if (ret <= 1 && ret > -1) {
      runForward(150, 2);
    } else if (ret <= -1 && ret > -2) {
      runRight(120, 2);
    } else if (ret <= -2 && ret >= -3) {
      runRight(150 ,2);
    } else if (ret < -3 ) {
      runRight(180, 2);
    }
  } else {
    runForward(150, 2000000);
  }
}


/* IR Sensor */

void initIR()
{
  int i;
  // setting input pins
  for (i = 0; i < IR_NUM; i++) {
    pinMode(ir_pin_array[i], INPUT);
  }
}

int readIR()
{
  int i;
  for (i = 0; i < IR_NUM; i++) {
     ir_array[i] = digitalRead(ir_pin_array[i]);
  }
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
    ret = p_term + i_term + d_term;
    /* TODO: check value range */
    return ret;
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

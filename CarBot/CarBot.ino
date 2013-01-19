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

/* collision */
#define PIN_COLLISION_IR 3

/* servo */
#define PIN_SERVO 2

#define IR_NUM 6
#define MOTOR_NUM 2
#define MOTOR_PWM 0
#define MOTOR_DIR1 1
#define MOTOR_DIR2 2
#define MOTOR_LEFT 0
#define MOTOR_RIGHT 1

#define PID_P_FACTOR 4.1
#define PID_I_FACTOR 0
#define PID_D_FACTOR 5
#define PID_SCALING_FACTOR 128

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
const int ir_pin_array[IR_NUM] = {
   PIN_IR1_LEFT, PIN_IR2_LEFT, PIN_IR3_LEFT, PIN_IR3_RIGHT, PIN_IR2_RIGHT, PIN_IR1_RIGHT
};
const int ir_pos_map[IR_NUM] = {
    -5, -3, -1, 1, 3, 5
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

void process()
{
  // get the position
  // process with PID
  // get result
  
  int i;
  int ret = 0;
  int ir_active_num = 0;
  int ir_pos_sum = 0;
  int ir_active = -1;
  int ir_active_left = -1;
  int ir_active_right = -1;
  
  if (collision_ir == LOW) {
    stopDead(1000);
    return;
  }
  
  for (i = 0; i < IR_NUM; i++) {
    int ir;
    //ir = i; // left most
    ir = (i + 2) % IR_NUM; // 8
    //ir = (i + IR_NUM - 1) % IR_NUM; // right most
    if (ir_array[ir] == LOW) {
      ir_active_num++;
      ir_active = (ir_active == -1) ? ir : ir_active;
      ir_pos_sum += ir_pos_map[ir];
    }
  }
  
  if (ir_active_num > 0) {
    ret = processPID(0, ir_pos_map[ir_active], &pid);
    runByPosition(ret, 1);
    /*
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
    */
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
    motorForward(MOTOR_LEFT, 170);
    motorForward(MOTOR_RIGHT, 10);
  } else if (position >= -5 && position < -1) {
    motorForward(MOTOR_LEFT, 170); 
    motorForward(MOTOR_RIGHT, 40 * position + 210);
  } else if (position >= -1 && position <= 1) {
    motorForward(MOTOR_LEFT, 170);
    motorForward(MOTOR_RIGHT, 170);
  } else if (position > 1 && position <= 5) {
    motorForward(MOTOR_LEFT, -40 * position + 210);
    motorForward(MOTOR_RIGHT, 170);
  } else if (position > 5) {
    motorForward(MOTOR_LEFT, 10);
    motorForward(MOTOR_RIGHT, 170);
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


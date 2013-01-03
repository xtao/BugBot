/*
 Main Program For BugBot
 Including IR sensor, Collision sensor, Motor, Servo
 
 Copyright @ InfiniteSense.com
*/

#define IR_NUM 6
#define COLLISION_NUM 2
#define MOTOR_NUM 2
#define SERVO_NUM 1

#define IR_INPUT_PIN_ARRAY 0
#define IR_CONTROL_PIN_ARRAY 1

#define MAX_INT INT16_MAX

#define PID_P_FACTOR 1
#define PID_I_FACTOR 0
#define PID_D_FACTOR 0

#define MOTOR_OUTPUT_MAX 1
#define MOTOR_OUTPUT_MIN 0

int ir_pin_array[2][IR_NUM] = {
  {A0, A1, A2, A3, A6, A7}, // analog input pins
  {4, 7, 8, 10, 11, 12} // digital control pins
};

int collision_pin_array[COLLISION_NUM] = {
  2, 3 // external interrupt pins
};

int motor_pin_array[MOTOR_NUM] = {
  5, 6 // digital PWM pins
};

int servo_pin_array[SERVO_NUM] = {
  9 // digital PWM pins
};

int ir_array[IR_NUM] = { 0 };
int motor_array[MOTOR_NUM] = { 0 };
int servo_array[SERVO_NUM] = { 0 };

int ir_pos_map[IR_NUM] = {
    -3, -2, -1, 1, 2, 3
};

int motor_right_a = 0;
int motor_right_b = 0;
int motor_left_a = 0;
int motor_left_b = 0;

struct pid_t {
  int p_factor;
  int i_factor;
  int d_factor;
  int sum_error;
  int last_error;
};

pid_t pid = {
  PID_P_FACTOR,
  PID_I_FACTOR,
  PID_D_FACTOR
};


void setup()
{
  Serial.begin(9600);
  initIR();
  initMotor();
  initServo();
}

void loop()
{
  // ...
}

void process() {
  
}

void initIR() {
  int i;
  // close light-emitting diodes on IR
  for (i = 0; i < IR_NUM; i++) {
    pinMode(ir_pin_array[IR_INPUT_PIN_ARRAY][i], INPUT);
    pinMode(ir_pin_array[IR_CONTROL_PIN_ARRAY][i], OUTPUT);
    digitalWrite(ir_pin_array[IR_CONTROL_PIN_ARRAY][i], HIGH);
  }
}

void initCollision() {
  int i;
  for (i = 0; i < COLLISION_NUM; i++) {
    //attachInterrupt(i, readCollision, CHANGE);
  }
}

void initMotor() {
  int i;
  for (i = 0; i < MOTOR_NUM; i++) {
    pinMode(motor_pin_array[i], OUTPUT); 
  }
}

void initServo() {
  int i;
  for (i = 0; i < SERVO_NUM; i++) {
    pinMode(servo_pin_array[i], OUTPUT); 
  }
}

// Read IR Values
int readIR()
{
  int i;
  int ir_diff_array[IR_NUM];
  
  // read the enviroment value
  for (i = 0; i < IR_NUM; i++) {
    ir_diff_array[i] = analogRead(ir_pin_array[IR_INPUT_PIN_ARRAY][i]);
  }
  
  // open light-emitting diodes on IR 
  for (i = 0; i < IR_NUM; i++) {
    digitalWrite(ir_pin_array[IR_CONTROL_PIN_ARRAY][i], LOW);
  }
  
  // wait...
  delay(500); // ms
  
  // read the diff ir value
  for (i = 0; i < IR_NUM; i++) {
    ir_diff_array[i] = analogRead(ir_pin_array[IR_INPUT_PIN_ARRAY][i]) - ir_diff_array[i];
    ir_array[i] = ir_diff_array[i];
  }  
  
  // close light-emitting diodes on IR
  for (i = 0; i < IR_NUM; i++) {
    digitalWrite(ir_pin_array[IR_CONTROL_PIN_ARRAY][i], HIGH);
  }
}

// Drive Motors
int driveMotor()
{
  int y = 0;
  int x = 0;
  y = motor_right_a * x + motor_right_b;
  y = (y > MOTOR_OUTPUT_MAX) ? MOTOR_OUTPUT_MAX : y ;
  y = (y < MOTOR_OUTPUT_MIN) ? MOTOR_OUTPUT_MIN : y ;
  y = motor_left_a * x + motor_left_b;
  y = (y > MOTOR_OUTPUT_MAX) ? MOTOR_OUTPUT_MAX : y ;
  y = (y < MOTOR_OUTPUT_MIN) ? MOTOR_OUTPUT_MIN : y ;

  int i;
  for (i = 0; i < MOTOR_NUM; i++) {
    analogWrite(motor_pin_array[i], motor_array[i]);
  }
}

int readCollision()
{
}

// Drive Servos
int driveServo()
{
  int i;
  for (i = 0; i < SERVO_NUM; i++) {
    analogWrite(servo_pin_array[i], servo_array[i]);
  }
}

int processPID(int setPoint, int processValue, struct pid_t *pid)
{
    int error = 0;
    int p_term;
    int i_term;
    int d_term;
    int ret;
    error = setPoint - processValue;
    p_term = pid->p_factor * error;
    pid->sum_error = pid->sum_error + error;
    i_term = pid->i_factor * pid->sum_error;
    d_term = pid->d_factor * (pid->last_error - error);
    pid->last_error = error;
    ret = p_term + i_term + d_term;
    return ret;
}

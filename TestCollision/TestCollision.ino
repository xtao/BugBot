/*
 Test Collision Sensor Program
 
 Copyright @ InfiniteSense.com
 */

#define COLLISION_PIN 2
#define COLLISION_INTERRUPT 0

volatile int state = LOW;

void setup() {
  Serial.begin(9600);
  initCollision();
}

void loop() {
  if (state == HIGH) {
    noInterrupts();
    state = LOW;
    interrupts();
    Serial.println("Checked");
  }
  delay(500);
}

void initCollision() {
  pinMode(COLLISION_PIN, INPUT);
  digitalWrite(COLLISION_PIN, HIGH);
  attachInterrupt(COLLISION_INTERRUPT, check, FALLING);
}

void check() {
  state = HIGH;
}

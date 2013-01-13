void setup() {
  // initialize the serial communication:
  Serial.begin(9600);
  
  // send the value of analog input 0:
  Serial.print("A0: ");
  Serial.println(A0);
  Serial.print("A1: ");
  Serial.println(A1);
  Serial.print("A2: ");
  Serial.println(A2);
  Serial.print("A3: ");
  Serial.println(A3);
  Serial.print("A4: ");
  Serial.println(A4);
  Serial.print("A5: ");
  Serial.println(A5);
  Serial.print("A6: ");
  Serial.println(A6);
  Serial.print("A7: ");
  Serial.println(A7);
}

void loop() {
  delay(1000);
}

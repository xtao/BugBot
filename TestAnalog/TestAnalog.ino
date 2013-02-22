#define PIN A0

int sensorValue;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  sensorValue = analogRead(PIN);
  Serial.println(sensorValue, DEC);
  delay(1000);
}

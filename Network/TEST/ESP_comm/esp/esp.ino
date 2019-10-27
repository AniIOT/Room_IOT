void setup()
{
  Serial.begin(38400);
}

void loop()
{
  while (!Serial.available())
  {
    Serial.println("not received");
    delay(100);
  }
  
    Serial.println(Serial.read());
}

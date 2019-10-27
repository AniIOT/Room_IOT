void setup() 
{
  Serial.begin(38400);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);
}

void loop() 
{
}

void serialEvent()
{
  Serial.println(Serial.read());
}

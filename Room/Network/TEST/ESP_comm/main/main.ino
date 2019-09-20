void setup()
{
  Serial3.begin(38400);
  Serial.begin(115200);
}

char c;

void loop()
{
  Serial3.write(130);
  Serial3.write(24);
  while (1);
}

void Sample()
{
  ct = 1;
  while (ct <= 8)
  {
    switch (ct)
    {
      case 1:
        digitalWrite(s1, 1);
        digitalWrite(s2, 1);
        digitalWrite(s3, 1);
        delayMicroseconds(50);
        sw1 = digitalRead(s0);
        sw1 = not sw1;
        break;
      case 2:
        digitalWrite(s1, 0);
        digitalWrite(s2, 1);
        digitalWrite(s3, 1);
        delayMicroseconds(50);
        sw2 = digitalRead(s0);
        sw2 = not sw2;
        break;
      case 3:
        digitalWrite(s1, 0);
        digitalWrite(s2, 0);
        digitalWrite(s3, 0);
        delayMicroseconds(50);
        sw3 = digitalRead(s0);
        sw3 = not sw3;
        break;
      case 4:
        digitalWrite(s1, 1);
        digitalWrite(s2, 0);
        digitalWrite(s3, 0);
        delayMicroseconds(50);
        sw4 = digitalRead(s0);
        sw4 = not sw4;
        break;
      case 5:
        digitalWrite(s1, 0);
        digitalWrite(s2, 1);
        digitalWrite(s3, 0);
        delayMicroseconds(50);
        sw5 = digitalRead(s0);
        sw5 = not sw5;
        break;
      case 6:
        digitalWrite(s1, 1);
        digitalWrite(s2, 1);
        digitalWrite(s3, 0);
        delayMicroseconds(50);
        sw6 = digitalRead(s0);
        sw6 = not sw6;
        break;
      case 7:
        digitalWrite(s1, 1);
        digitalWrite(s2, 0);
        digitalWrite(s3, 1);
        delayMicroseconds(100);
        sw7 = digitalRead(s0);
        break;
      case 8:
        digitalWrite(s1, 0);
        digitalWrite(s2, 0);
        digitalWrite(s3, 1);
        delayMicroseconds(100);
        sw8 = digitalRead(s0);
        break;
    }
    ct++;
  }
}

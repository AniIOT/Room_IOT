void VoiceCom()
{
  pt = millis();
  while (s != '<')
  {
    if (eSerial.available() > 0)
    {
      s = eSerial.read();
      if (s == '<')
        nextByteFlag = true;
    }
    tt = millis();
    if (tt >= pt + 50)
      break;
  }
  
  if (nextByteFlag)
  {
    while (s != '>')
    {
      if (eSerial.available() > 0)
      {
        s = eSerial.read();
        if (s != '>')
        {
          dat[received++] = s;
          dat[received] = '\0';
        }
      }
    }
  }
  nextByteFlag = false;
  buff = atoi(dat);

  if (buff > 0)
    com = buff;

//  Serial.println(com);
  
  received = 0;
  dat[received] = '\0';
  if ( com >= 200)
    rf00(com);
  switch (com)
  {
    case 11:
      if (a1 != 1)
        w1 = not w1;
      break;
    case 10:
      if (a1 != 0)
        w1 = not w1;
      break;
    case 21:
      if (a2 != 1)
        w2 = not w2;
      break;
    case 20:
      if (a2 != 0)
        w2 = not w2;
      break;
    case 31:
      if (a3 != 1)
        w3 = not w3;
      break;
    case 30:
      if (a3 != 0)
        w3 = not w3;
      break;
    case 41:
      if (a4 != 1)
        w4 = not w4;
      break;
    case 40:
      if (a4 != 0)
        w4 = not w4;
      break;
    case 51:
      if (a5 != 1)
        w5 = not w5;
      break;
    case 50:
      if (a5 != 1)
        w5 = not w5;
      break;
    case 61:
      if (a6 != 1)
        w6 = not w6;
      break;
    case 60:
      if (a6 != 0)
        w6 = not w6;
      break;
    case 71:
      if (a7 != 1)
        w7 = not w7;
      break;
    case 70:
      if (a7 != 0)
        w7 = not w7;
      break;
    case 81:
      if (a8 != 1)
        w8 = not w8;
      break;
    case 80:
      if (a8 != 0)
        w8 = not w8;
      break;
    case 111:
      if (a1 != 1)
        w1 = not w1;
      if (a8 != 1)
        w8 = not w8;
      break;
    case 110:
      if (a1 != 0)
        w1 = not w1;
      if (a8 != 0)
        w8 = not w8;
      break;
    case 121:
      if (a2 != 1)
        w2 = not w2;
      if (a3 != 1)
        w3 = not w3;
      if (a4 != 1)
        w4 = not w4;
      if (a5 != 1)
        w5 = not w5;
      break;
    case 120:
      if (a2 != 0)
        w2 = not w2;
      if (a3 != 0)
        w3 = not w3;
      if (a4 != 0)
        w4 = not w4;
      if (a5 != 0)
        w5 = not w5;
      break;
    case 131:
      if (a7 != 1)
        w7 = not w7;
      break;
    case 130:
      if (a7 != 0)
        w7 = not w7;
      break;
    case 141:
      if (a6 != 1)
        w6 = not w6;
      break;
    case 140:
      if (a6 != 0)
        w6 = not w6;
      break;
    case 300:
      if (a1 != 0)
        w1 = not w1;
      if (a2 != 0)
        w2 = not w2;
      if (a3 != 0)
        w3 = not w3;
      if (a4 != 0)
        w4 = not w4;
      if (a5 != 0)
        w5 = not w5;
      if (a6 != 0)
        w6 = not w6;
      if (a8 != 0)
        w8 = not w8;
      break;
    case 310:
      if (a1 != 0)
        w1 = not w1;
      if (a2 != 0)
        w2 = not w2;
      if (a3 != 0)
        w3 = not w3;
      if (a4 != 0)
        w4 = not w4;
      if (a5 != 0)
        w5 = not w5;
      if (a6 != 0)
        w6 = not w6;
      if (a7 != 0)
        w7 = not w7;
      if (a8 != 0)
        w8 = not w8;
      break;
    case 320:
      if (a1 != 1)
        w1 = not w1;
      if (a2 != 0)
        w2 = not w2;
      if (a3 != 0)
        w3 = not w3;
      if (a4 != 0)
        w4 = not w4;
      if (a5 != 0)
        w5 = not w5;
      if (a6 != 0)
        w6 = not w6;
      if (a8 != 1)
        w8 = not w8;
      break;
    case 330:
      if (a1 != 0)
        w1 = not w1;
      if (a2 != 1)
        w2 = not w2;
      if (a3 != 1)
        w3 = not w3;
      if (a4 != 1)
        w4 = not w4;
      if (a5 != 1)
        w5 = not w5;
      if (a6 != 0)
        w6 = not w6;
      if (a8 != 0)
        w8 = not w8;
      break;
    default:
      w1 = w1;
      w2 = w2;
      w3 = w3;
      w4 = w4;
      w5 = w5;
      w6 = w6;
      w7 = w7;
      w8 = w8;
      break;
  }
  com = 0;
}

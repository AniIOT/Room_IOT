void decoder(int incomingData)
{
  Serial.println(incomingData);
  switch (incomingData)
  {
    case 211:
      if (a1 != 1)
        rw1 = not rw1;
      break;
    case 210:
      if (a1 != 0)
        rw1 = not rw1;
      break;
    case 221:
      if (a2 != 1)
        rw2 = not rw2;
      break;
    case 220:
      if (a2 != 0)
        rw2 = not rw2;
      break;
    case 231:
      if (a3 != 1)
        rw3 = not rw3;
      break;
    case 230:
      if (a3 != 0)
        rw3 = not rw3;
      break;
    case 241:
      if (a4 != 1)
        rw4 = not rw4;
      break;
    case 240:
      if (a4 != 0)
        rw4 = not rw4;
      break;
    case 251:
      if (a5 != 1)
        rw5 = not rw5;
      break;
    case 250:
      if (a5 != 0)
        rw5 = not rw5;
      break;
    case 261:
      if (a6 != 1)
        rw6 = not rw6;
      break;
    case 260:
      if (a6 != 0)
        rw6 = not rw6;
      break;
    case 300:
      if (a1 != 0)
        rw1 = not rw1;
      if (a2 != 0)
        rw2 = not rw2;
      if (a3 != 0)
        rw3 = not rw3;
      if (a4 != 0)
        rw4 = not rw4;
      break;
    case 310:
      if (a1 != 0)
        rw1 = not rw1;
      if (a2 != 0)
        rw2 = not rw2;
      if (a3 != 0)
        rw3 = not rw3;
      if (a4 != 0)
        rw4 = not rw4;
      if (a5 != 0)
        rw5 = not rw5;
      if (a6 != 0)
        rw6 = not rw6;
      break;
    case 320:
      if (a2 != 1)
        rw2 = not rw2;
      break;
    default:
      rw1 = rw1;
      rw2 = rw2;
      rw3 = rw3;
      rw4 = rw4;
      rw5 = rw5;
      rw6 = rw6;
      break;
  }
  incomingData = 0;
}

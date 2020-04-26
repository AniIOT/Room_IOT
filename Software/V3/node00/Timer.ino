void TimedFunctionCaller_Init()
{
  currMillis = millis();
  prevMillis10000mstask = currMillis;
  prevMillis100mstask = currMillis;
  rxBufferCount = 0;
}

void TimedFunctionCaller()
{
  currMillis = millis();
  if (currMillis - prevMillis10000mstask >= 10000)
  {
    //    Serial.println("pinging");
    pingToServer();
    prevMillis10000mstask = currMillis;
  }

  if (currMillis - prevMillis100mstask >= 100)
  {
    ReadSwitches();
    prevMillis100mstask = currMillis;
  }
}

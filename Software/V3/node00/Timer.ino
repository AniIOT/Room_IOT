void TimedFunctionCaller_Init()
{
  currMillis = millis();
  prevMillis10000mstask = currMillis;
  prevMillis100mstask   = currMillis;
  prevMillis50mstask    = currMillis;
  rxBufferCount = 0;
}

void TimedFunctionCaller()
{
  currMillis = millis();

  if (currMillis - prevMillis50mstask >= 50)
  {
    ReadSwitches();
    prevMillis50mstask = currMillis;
  }

  if (currMillis - prevMillis100mstask >= 100)
  {
    MQTTStateMachine();
    prevMillis100mstask = currMillis;
  }

  if (currMillis - prevMillis10000mstask >= 10000)
  {
    if (MQTTInitFlag)
    {
    //    Serial.println("pinging");
      Serial.println(F("10s task"));
      pingToServer();
    }
    prevMillis10000mstask = currMillis;
  }
}

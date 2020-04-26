#include "node00.h"

static unsigned long prevMillis  = 0;
static unsigned long currMillis  = 0;

void setup()
{
  /*UART Init*/
  hal_uart_Init();
  delay(500);

  
  currMillis = millis();
  prevMillis = currMillis;
  rxBufferCount = 0;
}

void loop()
{
  /*MQTT_Init*/
  while (MQTTStateMachine() != eMQTTSuccess); //TODO: use assert param function to assert parameters for all functions
  
  currMillis = millis();
  if (currMillis - prevMillis >= 60000)
  {
//    Serial.println("pinging");
    pingToServer();
    prevMillis = currMillis;
  }
//  for(uint8_t i = 0; i < MaxSwitches; i++)
//  {
//    Serial.print(MQTTSwitchBuffer[i]);
//  }
//  Serial.println("");
}
#include "node00.h"

void setup()
{
  /*UART Init*/
  hal_uart_Init();
  delay(500);       //TODO: Check if necessary

  /*Timer Init*/
  TimedFunctionCaller_Init();
}

void loop()
{
  /*MQTT_Init*/
  while (MQTTStateMachine() != eMQTTSuccess); //TODO: use assert param function to assert parameters for all functions

  //Publish switch status if mood request is received
  publishIfMoodReq();

  //Call all functions according to alloted time
  TimedFunctionCaller();

  
  //  for(uint8_t i = 0; i < MaxSwitches; i++)
  //  {
  //    Serial.print(MQTTSwitchBuffer[i]);
  //  }
  //  Serial.println("");
}

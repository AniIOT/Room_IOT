#include "remote.h"

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
  //Call all functions according to alloted time
  TimedFunctionCaller();

  //  for(uint8_t i = 0; i < MaxSwitches; i++)
  //  {
  //    Serial.print(MQTTSwitchBuffer[i]);
  //  }
  //  Serial.println("");
}

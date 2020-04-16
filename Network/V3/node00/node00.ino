#include "node00.h"
#include "stdint.h"

void setup()
{
  /*UART Init*/
  hal_uart_Init();
  delay(500);

  /*ESP Init*/
  while (ESPStateMachine() != eEspSuccess);

//  /*MQTT_Init*/
//  while (MQTTStateMachine() != eMQTTSuccess);
connectToBroker();
  
}

void loop()
{

}

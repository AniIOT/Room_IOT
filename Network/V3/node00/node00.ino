#include "node00.h"
#include "stdint.h"

void setup()
{
  /*UART Init*/
  hal_uart_Init();
  delay(500);

  /*MQTT_Init*/
  while (MQTTStateMachine() != eMQTTSuccess);
}

void loop()
{
  pingToServer();
  delay(700);
}

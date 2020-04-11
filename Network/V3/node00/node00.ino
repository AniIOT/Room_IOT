
#include "node00.h"
#include "hal_uart.h"
#include "esp.h"

void setup()
{
  hal_uart_Init();
  delay(500);
}

void loop()
{
  if (ESPStateMachine() == eEspSuccess)
    Serial.print("SUCCESS\r\n");
  delay(500);
}

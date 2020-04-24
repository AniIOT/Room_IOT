#include "node00.h"
#include "stdint.h"

static unsigned long prevMillis  = 0;
static unsigned long currMillis  = 0;

void setup()
{
  /*UART Init*/
  hal_uart_Init();
  delay(500);

  /*MQTT_Init*/
  while (MQTTStateMachine() != eMQTTSuccess);
  
  currMillis = millis();
  prevMillis = currMillis;
  rxBufferCount = 0;
}

void loop()
{
  currMillis = millis();
  if (currMillis - prevMillis >= 1000)
  {
//    Serial.println("pinging");
    pingToServer();
    prevMillis = currMillis;
  }
}

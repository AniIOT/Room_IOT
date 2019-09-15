#include "mqtt.h"

void mqtt_init()
{
  mqtt.subscribe(&Lights);
}

void mqtt_getData(uint16_t* pu16Data)
{
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(500)))
  {
    if (subscription == &Lights)
    {
      *pu16Data = atoi((char *)Lights.lastread);
    }
  }
}

void mqtt_connect()
{
  int8_t ret;

  if (mqtt.connected())
  {
    return;
  }

  while ((ret = mqtt.connect()) != 0)
  {
    mqtt.disconnect();
    delay(5000);
  }
}

void mqtt_checkConnection()
{
  if (! mqtt.ping())
  {
    mqtt.disconnect();
  }
}

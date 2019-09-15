#include "esp_main.h"

boolean check = 0;
unsigned long setCount = 0;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe Lights = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LightD");

void setup()
{
  /*Serial UART initialization*/
  uart_init();

  /*WiFi initialization*/
  wifi_init();

  /*MQTT feed initialization*/
  mqtt_init();

  /*Timeout timer initialization*/  /*will be replaced by watchdog*/
  setCount = millis();
}

void loop()
{
  /*Get-MQTT-Data*/
  mqtt_connect();
  mqtt_getData(pu16Data);

  /*Send-Data-to-MuC-if-Requested*/
  check = modbusCheckReq(RxBuffer);
  if (check == 1)
  {
    modbusSendData(pu16Data);
    check = 0;
  }

  /*Health-Check*/
  if (rxCount > rxmaxBytes)
  {
    rxCount = 0;
  }
  checkTimeout();
  mqtt_checkConnection();
}

void checkTimeout() //shall be replaced by watchdog timer
{
  unsigned long currentCount = 0;
  currentCount = millis();
  if ( currentCount >= setCount + 150000) //5 second timeout
  {
    ESP.restart();
  }
}

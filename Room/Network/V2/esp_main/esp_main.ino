#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include "ESP8266WiFi.h"

#define WLAN_SSID       "Private"
#define WLAN_PASS       "A342320D"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "AniIOT"
#define AIO_KEY         "1d2f2a184b57420e9f95b216d614a181"

#define uartBaud 38400
#define txmaxBytes 4
#define rxmaxBytes 2
#define slave_add 130
#define Func_Code 24

/*Global-Variables*/
boolean check = 0;
boolean newData = 0;
unsigned long setCount = 0;
unsigned char RxBuffer[rxmaxBytes] = {0};
uint8_t rxCount = 0;

/*local-variables*/
uint16_t u16Data = 0, m16Data = 0, pm16Data = 0;

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe Lights = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LightD", MQTT_QOS_1);

void setup()
{
  Serial.begin(uartBaud);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  mqtt.subscribe(&Lights);

  /*Timeout timer initialization*/  /*will be replaced by watchdog*/
  setCount = millis();
}

void loop()
{
  unsigned long currentCount = 0;
  currentCount = millis();
  if ( currentCount >= setCount + 150000) //5 second timeout
  {
    ESP.restart();
  }

  /*Get-MQTT-Data*/
  mqtt_connect();
  pm16Data = m16Data;
  while (pm16Data == m16Data)
  {
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(500)))
    {
      if (subscription == &Lights)
      {
        m16Data = atoi((char *)Lights.lastread);
      }
    }
    /*Health-Check*/
    if (!mqtt.ping())
    {
      mqtt.disconnect();
    }

  }

  /*Send-Data-to-MuC*/
  modbusSendData(m16Data);

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

void modbusSendData(uint16_t u16Data)
{
  unsigned char u8TxBuffer[txmaxBytes] = {0};
  unsigned char i = 0;

  u8TxBuffer[i++] = slave_add;
  u8TxBuffer[i++] = Func_Code;
  u8TxBuffer[i++] = (unsigned char)(u16Data & 0x00ff); //lsB
  u8TxBuffer[i] = (unsigned char)((u16Data >> 8)); //msB

  tx_uart(u8TxBuffer, i);
}

void tx_uart(unsigned char *packet, uint16_t packetlength)
{
  uint16_t iBytes = 0;
  unsigned char dataBuff;
  unsigned char* pTxbuf = packet;

  while (iBytes <= packetlength)
  {
    dataBuff = *(pTxbuf++);
    Serial.write((unsigned char)dataBuff);
    iBytes++;
    Serial.flush();
    delay(1);
  }
}

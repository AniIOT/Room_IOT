#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "Private"
#define WLAN_PASS       "A342320D"

/************************* Adafruit.io Setup *********************************/
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
uint8_t rxCount = 0;

/*local-variables*/
uint16_t u16Data = 0, data_len = 0;
char m16Data[5] = {0};

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe Lights = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LightD", MQTT_QOS_1);

void mqtt_connect();
void modbusSendData(uint16_t u16Data);
void tx_uart(unsigned char *packet, uint16_t packetlength);

void lightscallback(char *data, uint16_t len) 
{
  while (data_len < len)
  {
    m16Data[data_len++] = *data++;
  }
  m16Data[data_len] = '\0';
  u16Data = atoi(m16Data);
  data_len = 0;
}

void setup()
{
  Serial.begin(uartBaud);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Lights.setCallback(lightscallback);
  mqtt.subscribe(&Lights);
}

void loop()
{
  while(u16Data == 0)
  {
    mqtt_connect();
    mqtt.processPackets(2000);
    mqtt.ping();
  }

  /*Send-Data-to-MuC*/
  if (u16Data != 0)
  {
    modbusSendData(u16Data);
    u16Data = 0;
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
    Serial.print((unsigned char)dataBuff);
    iBytes++;
    Serial.flush();
    delay(1);
  }
}

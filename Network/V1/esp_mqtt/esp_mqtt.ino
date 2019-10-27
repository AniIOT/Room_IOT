#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#define WLAN_SSID       "Private"
#define WLAN_PASS       "A342320D"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "AniIOT"
#define AIO_KEY         "1d2f2a184b57420e9f95b216d614a181"

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe Lights = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/LightD");

void MQTT_connect();

char dat[4];
int received = 0;
int com, buff;
unsigned long ct = 0, pt = 0, tt = 0;

void setup()
{
  Serial.begin(115200);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  mqtt.subscribe(&Lights);
  pt = millis();
}

void loop()
{
  MQTT_connect();
  ct = millis();
  if ( ct >= pt + 150000)
    ESP.restart();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000)))
  {
    if (subscription == &Lights)
    {
      if (atoi((char *)Lights.lastread) > 0)
      {
        Serial.print('<');
        Serial.write((char *)Lights.lastread);
        Serial.print('>');
        Serial.flush();
      }
    }
  }
  if (! mqtt.ping())
    mqtt.disconnect();
}
void MQTT_connect() {
  int8_t ret;

  if (mqtt.connected()) {
    return;
  }

  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    delay(5000);
  }
}

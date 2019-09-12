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

#define relay D5

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe timefeed = Adafruit_MQTT_Subscribe(&mqtt, "time/seconds");

void timecallback(uint32_t current)
{
  uint32_t epoch = current;
  byte second = epoch%60; epoch /= 60;
  byte minute = epoch%60; epoch /= 60;
  byte hour   = epoch%24; epoch /= 24;
  hour += 5;
  minute += 30;
  if ( minute >= 60)
  {
    hour += 1;
    minute -= 60;
  }
  Serial.print(hour);
  Serial.print(" : ");
  Serial.print(minute);
  Serial.print(" : ");
  Serial.println(second);
  if (hour >= 9 && hour < 16)
  {
    Serial.println("yayy");
    digitalWrite(relay, HIGH); //Relay off
  }
  if (hour >= 16)
  {
    digitalWrite(relay, LOW);  //Relay on
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F(" WiFi connected."));

  timefeed.setCallback(timecallback);
  mqtt.subscribe(&timefeed);
}

void loop() {
  MQTT_connect();
  mqtt.processPackets(10000);
  mqtt.ping();
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);
    retries--;
    if (retries == 0) {
      while (1);
    }
  }

  Serial.println("MQTT Connected!");
}

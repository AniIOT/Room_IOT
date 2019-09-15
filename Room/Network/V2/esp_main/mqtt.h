#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "wifi.h"

#define WLAN_SSID       "Private"
#define WLAN_PASS       "A342320D"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "AniIOT"
#define AIO_KEY         "1d2f2a184b57420e9f95b216d614a181"

void mqtt_init();
void mqtt_getData(uint16_t* pu16Data);
void mqtt_connect();
void mqtt_checkConnection();

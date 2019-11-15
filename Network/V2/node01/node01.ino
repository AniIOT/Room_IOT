#include "node01.h"
#include "rf_comm.h"
#include "wifi_comm.h"
#include "gpio.h"
#include "uart.h"
#include "wdg.h"
#include "storage.h"

boolean wifiBuffer[16] = {0};
boolean switchBuffer[8] = {0};
boolean rfBuffer[8] = {0};

void setup()
{
  //gpio initialization
  gpio_init();

  //nrf-spi initialization
  rf_init();

  //wifi-uart initialization
  uart_init();

  //watchdog initialization
  wdg_init();
}

void loop()
{
  switch (machine_state)
  {
    case readStorage:
      getStored();
      machine_state = readSwitches;
    break;
    
    case readSwitches:  //read current state of switches
      read_switches();
      machine_state = readWifiData;
      break;

    case readWifiData: //read and process data from mqtt broker
//      wifi_comm();
      machine_state = writetoRelays;
      break;

    case writetoRelays:  //write final value to appliances
      write_Relays();
      machine_state = writetoRF;
      break;

    case writetoRF: //send apt data to panel 2
      rf_comm();
      machine_state = readSwitches;
      break;

//    case writetoMem: //store current state in eeprom /*Note: Can be replaced by watchdog ISR*/
//      storeCurrent();
//      break;
  }
  wdg_reset();
}

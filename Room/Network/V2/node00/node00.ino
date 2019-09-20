#include "node00.h"

void setup()
{
  //gpio initialization
  gpio_init();
  
  //nrf-spi initialization
  //  spi_init();

  //wifi-uart initialization
  //  uart_init();

  //watchdog initialization
  //  wdg_init();
#if testenable
  Serial.begin(115200);
  pinMode(2, INPUT);
  Serial.println("Test Mode");
#endif

}

void loop()
{
#if testenable
    read_switches();
    Serial.println("");
    Serial.print(SwitchRead_Buffer[0]);
    Serial.print(SwitchRead_Buffer[1]);
    Serial.print(SwitchRead_Buffer[2]);
    Serial.print(SwitchRead_Buffer[3]);
    Serial.print(SwitchRead_Buffer[4]);
    Serial.print(SwitchRead_Buffer[5]);
    Serial.print(SwitchRead_Buffer[6]);
    Serial.print(SwitchRead_Buffer[7]);
#else
  switch (machine_state)
  {
    case readSwitches:  //read current state of switches
      read_switches();
      machine_state = readWifiData;
      break;

    case readWifiData: //request from esp and read result
      //      wifi_comm();
      //      if () //if panel 1
      //      {
      //        machine_state = writetoRelays;
      //      }
      //
      //      if  //if panel 2
      //      {
      //        rf_comm();
      machine_state = writetoRelays;
      //      }
      break;

    case writetoRelays:  //write final value to appliances
      write_Relays();
      wdg_reset();
      machine_state = readSwitches;
      break;
  }

#endif
}

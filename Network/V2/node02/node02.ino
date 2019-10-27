#include "node01.h"

void setup()
{
  //gpio initialization
  gpio_init();

  //nrf-spi initialization
  //  spi_init();

  //watchdog initialization
  //  wdg_init();
#if testenable
  //  pinMode(8,INPUT);
  //  pinMode(7,INPUT);
  //  pinMode(6,INPUT);
  //  pinMode(5,INPUT);
  //  pinMode(4,INPUT);
  Serial.begin(115200);
  Serial.println("Test Mode");
#endif
  Serial.begin(115200);
}

void loop()
{
#if testenable
  read_switches();
  Serial.print(SwitchRead_Buffer[0]);
  Serial.print(SwitchRead_Buffer[1]);
  Serial.print(SwitchRead_Buffer[2]);
  Serial.print(SwitchRead_Buffer[3]);
  Serial.println(SwitchRead_Buffer[4]);
  write_Relays();

  //  Serial.print(digitalRead(8));
  //  Serial.print(digitalRead(7));
  //  Serial.print(digitalRead(6));
  //  Serial.print(digitalRead(5));
  //  Serial.println(digitalRead(4));
#else
  switch (machine_state)
  {
    case readSwitches:  //read current state of switches
      read_switches();
      machine_state = readRfData;
      break;

    case readRfData: //request from nrf and process data
      rf_comm();
      machine_state = writetoRelays;
      break;

    case writetoRelays:  //write final value to appliances
      write_Relays();
      wdg_reset();
      machine_state = readSwitches;
      break;
  }
#endif
}

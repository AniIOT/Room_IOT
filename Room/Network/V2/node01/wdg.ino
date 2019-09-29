#include <avr/wdt.h>

void wdg_init()
{
  wdt_disable();
  delay(2200);
  wdt_enable(WDTO_2S); 
}

void wdg_reset()
{
  wdt_reset();
}

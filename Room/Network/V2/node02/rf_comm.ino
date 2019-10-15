#include "gpio.h"

void spi_init()
{
  RF24 radio(GPIOB_CE, GPIOB_CSN);
  RF24Network network(radio);
  radio.begin();
  network.begin(90, THIS_NODE);
}

void rf_comm()
{
  
}

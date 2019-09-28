#include "gpio.h"
#include "rf_comm.h"

void rf_init()
{
  RF24 radio(GPIOB_CE, GPIOB_CSN);
  RF24Network network(radio);
  radio.begin();
  network.begin(90, THIS_NODE);
}

void rf_comm()
{
  memcpy(rfBuffer,&wifiBuffer[8],8*sizeof(boolean));
  //send rfBuffer
  
}

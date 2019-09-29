#include "gpio.h"
#include "rf_comm.h"

RF24 radio(GPIOB_CE, GPIOB_CSN);
RF24Network network(radio);

void rf_init()
{
  radio.begin();
  network.begin(90, THIS_NODE);
}

void rf_comm()
{
  memcpy(rfBuffer, &wifiBuffer[8], 8 * sizeof(boolean));
  //send rfBuffer
  uint8_t rf8Data = convertoToInt(rfBuffer);
  network.update();
  RF24NetworkHeader header(THIS_NODE);
  bool ok = network.write(header, &rf8Data, sizeof(rf8Data));
}

uint8_t convertoToInt(boolean* rfBuffer)
{
  uint8_t tempData = 0;
  for (uint8_t i = 0; i < RF_DATA_LEN; i++)
  {
    if (rfBuffer[i])
      tempData += pow(2, i);
  }
}

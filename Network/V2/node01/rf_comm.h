#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

#define THIS_NODE         ((uint8_t)0x00)
#define RF_DATA_LEN       ((uint8_t)0x08)

void rf_init();
void rf_comm();
uint8_t convertoToInt(boolean* rfBuffer);

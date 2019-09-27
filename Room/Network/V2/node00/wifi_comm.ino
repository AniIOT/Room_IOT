#include "wifi_comm.h"
#include "uart.h"

#define dataLength 16

/*Gloabal variables*/
uint16_t wData = 0;

bool wifi_comm()
{
//  if (checkData())
//  {
    wData |= 4096;
    memcpy(wifiBuffer, &wData, dataLength);
    Serial.println(wifiBuffer[12]);
    return true;
//  }
//  else
//  {
//    return false;
//  }
}

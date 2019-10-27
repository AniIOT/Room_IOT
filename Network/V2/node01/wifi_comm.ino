#include "wifi_comm.h"
#include "uart.h"

#define dataLength 16

/*Gloabal variables*/

bool wifi_comm()
{
  uint16_t wData = 0;
  if (checkData() == true)
  {
    wData = getData();
    parseData(wData);
    return true;
  }
  else
  {
    return false;
  }
}

void parseData(uint16_t wu16Data)
{
  bool sceneFlag = false;
  sceneFlag = checkforScene(wu16Data);
  if (sceneFlag == false) //if not set only one appliance needs to be toggled
  {
    for (uint16_t i = 0; i < dataLength; i++)
    {
      if (wu16Data % 2)
      {
        wifiBuffer[i] = not wifiBuffer[i];
        break;
      }
      wu16Data /= 2;
    }
  }
  else //all the appliances are to be switched according to data
  {
    for (uint16_t i = 0; i < dataLength; i++)
    {
      wifiBuffer[i] = wu16Data % 2;
      wu16Data /= 2;
    }
  }
}

bool checkforScene(uint16_t wu16Data) //if more than one 1 send true else false
{
  uint8_t count = 0;
  while (wu16Data > 0)
  {
    if (wu16Data % 2)
    {
      count++;
    }
    wu16Data /= 2;
  }
  if (count > 1)
    return true;
  else
    return false;
}

void printBuffer()
{
  Serial.print(wifiBuffer[0]);
  Serial.print(wifiBuffer[1]);
  Serial.print(wifiBuffer[2]);
  Serial.print(wifiBuffer[3]);
  Serial.print(wifiBuffer[4]);
  Serial.print(wifiBuffer[5]);
  Serial.print(wifiBuffer[6]);
  Serial.print(wifiBuffer[7]);
  Serial.print(wifiBuffer[8]);
  Serial.print(wifiBuffer[9]);
  Serial.print(wifiBuffer[10]);
  Serial.print(wifiBuffer[11]);
  Serial.print(wifiBuffer[12]);
  Serial.print(wifiBuffer[13]);
  Serial.print(wifiBuffer[14]);
  Serial.println(wifiBuffer[15]);
}

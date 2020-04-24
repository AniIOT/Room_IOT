#include <NeoSWSerial.h>

/*Software Serial Baudrate*/
#define ESP_UART_BAUD                   9600

/*Hardware Serial Baudrate*/
#define DEBUG_UART_BAUD                 9600

/*Software UART pins - ESP8266 pins*/
#define ESP_RX                          ((uint8_t)0x04)  //esp uart        D4
#define ESP_TX                          ((uint8_t)0x05)  //esp uart        D5

NeoSWSerial eSerial(ESP_RX, ESP_TX);    //Software serial object

void hal_uart_Init()
{
  eSerial.attachInterrupt(eRxInterruptHandler);
  eSerial.begin(ESP_UART_BAUD);
  Serial.begin(DEBUG_UART_BAUD);
}

void clearRxBuffer()
{
  rxBufferCount = 0;
  for (uint8_t i = 0; i < RxMaxBuffSize; i++)
  {
    RxBuffer[i] = 0;
  }
}

boolean hal_uart_tx(char* pTxBuff, uint8_t utxCount)
{
  clearRxBuffer();
  for (uint8_t i = 0; i < utxCount; i++)
    eSerial.write(*pTxBuff++);
  return true;
}

static void eRxInterruptHandler(uint8_t rxChar)
{
  if (!(ESPInitFlag && MQTTInitFlag))
  {
    if ((rxChar != '\n'))
    {
      RxBuffer[rxBufferCount++] = (unsigned char)rxChar;
      //              Serial.print(rxBufferCount-1);
      //      Serial.println(RxBuffer[rxBufferCount - 1], HEX);
    }
  }
  else
  {
    if (!((rxBufferCount < 2) && ((rxChar == MQTT_CTRL_PINGRESP) || (rxChar == 0x00)))) //To ignore ping response
      RxBuffer[rxBufferCount++] = (unsigned char)rxChar;
    //    Serial.print(rxBufferCount - 1);
    //    Serial.println((char)RxBuffer[rxBufferCount - 1]);

    if ((RxBuffer[0] == (MQTT_CTRL_PUBLISH | MQTT_CTRL_PUBLISH_FLAG)))
    {
      if (rxBufferCount > (RxBuffer[1] + 1))
      {
        hal_uart_decodeData();
        rxBufferCount = 0;
      }
    }
    else
      rxBufferCount = 0;
  }
  if (rxBufferCount >= RxMaxBuffSize)
    rxBufferCount = 0;
}

void hal_uart_decodeData()
{
  if (RxBuffer[17] == 'm')                                        //Main Room
  {
    if (RxBuffer[22] == 'f')                                      //Ceiling Fan
    {
      if (RxBuffer[25] == 'N')
        MQTTSwitchBuffer[Fan] = true;
      else
        MQTTSwitchBuffer[Fan] = false;
    }
    else if (RxBuffer[22] == 'l')               //Yellow light 1
    {
      if (RxBuffer[23] == '1')
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[Yellow1] = true;
        else
          MQTTSwitchBuffer[Yellow1] = false;
      }
      if (RxBuffer[23] == '2')               //Yellow light 2
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[Yellow2] = true;
        else
          MQTTSwitchBuffer[Yellow2] = false;
      }
      if (RxBuffer[23] == '3')               //White light 1
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[White1] = true;
        else
          MQTTSwitchBuffer[White1] = false;
      }
      if (RxBuffer[23] == '4')               //White light 2
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[White2] = true;
        else
          MQTTSwitchBuffer[White2] = false;
      }
      if (RxBuffer[23] == '5')               //White light 3
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[White3] = true;
        else
          MQTTSwitchBuffer[White3] = false;
      }
      if (RxBuffer[23] == '6')               //White light 4
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[White4] = true;
        else
          MQTTSwitchBuffer[White4] = false;
      }
    }
    else if (RxBuffer[22] == 'a' && RxBuffer[23] == 'c')               //AC
    {
      Serial.print((char)RxBuffer[24]);
      Serial.println((char)RxBuffer[25]);
    }
  }
  else if (RxBuffer[17] == 'p')                                   //Passage
  {
    if (RxBuffer[22] == 'l')              //Passage light
    {
      if (RxBuffer[23] == '1')
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[Passage] = true;
        else
          MQTTSwitchBuffer[Passage] = false;
      }
      if (RxBuffer[23] == '2')               //Loft light
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[Loft] = true;
        else
          MQTTSwitchBuffer[Loft] = false;
      }
    }
  }
  else if (RxBuffer[17] == 't')                                   //Toilet
  {
    if (RxBuffer[22] == 'f')               //Exhaust Fan
    {
      if (RxBuffer[25] == 'N')
        MQTTSwitchBuffer[Exaust] = true;
      else
        MQTTSwitchBuffer[Exaust] = false;
    }
    if (RxBuffer[22] == 'l')
    {
      if (RxBuffer[23] == '1')               //Toilet white light
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[ToiletWhite] = true;
        else
          MQTTSwitchBuffer[ToiletWhite] = false;
      }
      if (RxBuffer[23] == '2')               //Toilet yellow light
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[ToiletYellow] = true;
        else
          MQTTSwitchBuffer[ToiletYellow] = false;
      }
      if (RxBuffer[23] == '3')               //Toilet water heater
      {
        if (RxBuffer[25] == 'N')
          MQTTSwitchBuffer[WaterHeater] = true;
        else
          MQTTSwitchBuffer[WaterHeater] = false;
      }
    }
  }
  else if (RxBuffer[17] == 'M')                                  //MOOD
  {
    if (RxBuffer[21] == 'c')                                     //Chill Mood
      Serial.println("Chill Mood");
    else if (RxBuffer[21] == 'w')                                //Work Mood
      Serial.println("Work Mood");
    else if (RxBuffer[21] == 'o')                                //All off
      Serial.println("All OFF");
  }
}

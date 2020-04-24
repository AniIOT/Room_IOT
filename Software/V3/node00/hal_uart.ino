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
    if ((rxChar != '\n') and (rxBufferCount < RxMaxBuffSize))
    {
      RxBuffer[rxBufferCount++] = (unsigned char)rxChar;
      //              Serial.print(rxBufferCount-1);
      //      Serial.println(RxBuffer[rxBufferCount - 1], HEX);
    }
  }
  else
  {
    if (!((rxBufferCount < 2) && ((rxChar == MQTT_CTRL_PINGRESP) || (rxChar == 0x00))))
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
}

void hal_uart_decodeData()
{
  if (RxBuffer[17] == 'm')                                        //Main Room
  {
    if (RxBuffer[22] == 'f')                                      //Ceiling Fan
    {
      //      if (RxBuffer[25] == 'N')
      //        Serial.println("Fan ON");
      //      else
      //        Serial.println("Fan OFF");
    }
    else if (RxBuffer[22] == 'l')               //Yellow light 1
    {
      if (RxBuffer[23] == '1')
      {
        //        if (RxBuffer[25] == 'N')
        //          Serial.println("First Yellow Light ON");
        //        else
        //          Serial.println("First Yellow Light OFF");
      }
      if (RxBuffer[23] == '2')               //Yellow light 2
      {
        //        if (RxBuffer[25] == 'N')
        //          Serial.println("Second Yellow Light ON");
        //        else
        //          Serial.println("Second Yellow Light OFF");
      }
      if (RxBuffer[23] == '3')               //White light 1
      {
        //        if (RxBuffer[25] == 'N')
        //          Serial.println("White 1 ON");
        //        else
        //          Serial.println("White 1 OFF");
      }
      if (RxBuffer[23] == '4')               //White light 2
      {
        //        if (RxBuffer[25] == 'N')
        //          Serial.println("White 2 ON");
        //        else
        //          Serial.println("White 2 OFF");
      }
      if (RxBuffer[23] == '5')               //White light 3
      {
        //        if (RxBuffer[25] == 'N')
        //          Serial.println("White 3 ON");
        //        else
        //          Serial.println("White 3 OFF");
      }
      if (RxBuffer[23] == '6')               //White light 4
      {
        //        if (RxBuffer[25] == 'N')
        //          Serial.println("White 4 ON");
        //        else
        //          Serial.println("White 4 OFF");
      }
    }
    else if (RxBuffer[22] == 'A' && RxBuffer[23] == 'C')               //AC
    {
      //      if (RxBuffer[25] == 'N')
      //        Serial.println("AC ON");
      //      else
      //        Serial.println("AC OFF");
    }
  }
  else if (RxBuffer[17] == 'p')                                   //Passage
  {
    if (RxBuffer[22] == 'l')              //Passage light
    {
      if (RxBuffer[23] == '1')
      {
        //      if (RxBuffer[25] == 'N')
        //        Serial.println("Passage ON");
        //      else
        //        Serial.println("Passage OFF");
      }
      if (RxBuffer[23] == '2')               //Loft light
      {
        //      if (RxBuffer[25] == 'N')
        //        Serial.println("Loft ON");
        //      else
        //        Serial.println("Loft OFF");
      }
    }
  }
  else if (RxBuffer[17] == 't')                                   //Toilet
  {
    if (RxBuffer[22] == 'f')               //Exhaust Fan
    {
      //      if (RxBuffer[25] == 'N')
      //        Serial.println("Exaust ON");
      //      else
      //        Serial.println("Exaust OFF");
    }
    if (RxBuffer[22] == 'l')
    {
      if (RxBuffer[23] == '1')               //Toilet white light
      {
        //      if (RxBuffer[25] == 'N')
        //        Serial.println("Toilet White Lights ON");
        //      else
        //        Serial.println("Toilet White Lights OFF");
      }
      if (RxBuffer[23] == '2')               //Toilet yellow light
      {
        //      if (RxBuffer[25] == 'N')
        //        Serial.println("Toilet Yellow Lights ON");
        //      else
        //        Serial.println("Toilet Yellow Lights OFF");
      }
      if (RxBuffer[23] == '3')               //Toilet water heater
      {
        //      if (RxBuffer[25] == 'N')
        //        Serial.println("Water Heater ON");
        //      else
        //        Serial.println("Water Heater OFF");
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

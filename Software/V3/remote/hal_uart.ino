
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
  if (!MQTTInitFlag)
  {
    if ((rxChar != '\n'))
    {
      RxBuffer[rxBufferCount++] = (unsigned char)rxChar;
      //              Serial.print(rxBufferCount-1);
      //      Serial.println(RxBuffer[rxBufferCount - 1], HEX);
    }
  }
  //  else
  //  {
  //    if (!((rxBufferCount < 2) && ((rxChar == MQTT_CTRL_PINGRESP) || (rxChar == 0x00)))) //To ignore ping response
  //    {
  //      RxBuffer[rxBufferCount++] = (unsigned char)rxChar;
  //    }
  //    else
  //      u8pingCount = 0;
  //    //    Serial.print(rxBufferCount - 1);
  //    //    Serial.println((char)RxBuffer[rxBufferCount - 1]);
  //
  //    if ((RxBuffer[0] == (MQTT_CTRL_PUBLISH | MQTT_CTRL_PUBLISH_FLAG)))
  //    {
  //      if (rxBufferCount > (RxBuffer[1] + 1))
  //      {
  //        hal_uart_decodeData();
  //        rxBufferCount = 0;
  //      }
  //    }
  //    else
  //      rxBufferCount = 0;
  //  }
  if (rxBufferCount >= RxMaxBuffSize)
    rxBufferCount = 0;
}

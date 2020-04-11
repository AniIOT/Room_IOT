#include <NeoSWSerial.h>
#define ESPuartBaud 9600

static uint8_t rxBufferCount = 0;

/*Software UART pins - ESP8266 pins*/
#define ESP_RX                 ((uint8_t)0x04)  //esp uart        D4
#define ESP_TX                 ((uint8_t)0x05)  //esp uart        D5

NeoSWSerial eSerial(ESP_RX, ESP_TX);

void hal_uart_Init()
{
  eSerial.attachInterrupt(eRxInterruptHandler);
  eSerial.begin(ESPuartBaud);
  Serial.begin(ESPuartBaud);
}

static void eRxInterruptHandler(uint8_t rxChar)
{
  if ((rxChar != '\n') and (rxBufferCount < ESPRxMaxBuffSize))
  {
    ESPRxBuffer[rxBufferCount++] = (char)rxChar;
//    Serial.print(ESPRxBuffer[rxBufferCount-1]);
//    Serial.print(rxBufferCount);
  }
}

void clearRxBuffer()
{
  rxBufferCount = 0;
  for(uint8_t i = 0; i < ESPRxMaxBuffSize; i++)
  {
    ESPRxBuffer[i] = 0;
  }
}

boolean hal_uart_tx(char* pTxBuff, uint8_t utxCount)
{
  clearRxBuffer();
  for(uint8_t i = 0; i < utxCount; i++)
    eSerial.write(*pTxBuff++);
}

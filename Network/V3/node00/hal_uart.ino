#include <NeoSWSerial.h>

/*Software Serial Baudrate*/
#define ESP_UART_BAUD                   9600

/*Hardware Serial Baudrate*/
#define DEBUG_UART_BAUD                 9600

/*Software UART pins - ESP8266 pins*/
#define ESP_RX                 ((uint8_t)0x04)  //esp uart        D4
#define ESP_TX                 ((uint8_t)0x05)  //esp uart        D5

NeoSWSerial eSerial(ESP_RX, ESP_TX);

static uint8_t rxBufferCount = 0;

void hal_uart_Init()
{
  eSerial.attachInterrupt(eRxInterruptHandler);
  eSerial.begin(ESP_UART_BAUD);
  Serial.begin(DEBUG_UART_BAUD);
}

static void eRxInterruptHandler(uint8_t rxChar)
{
//  if (ESPInitFlag)
    if ((rxChar != '\n') and (rxBufferCount < ESPRxMaxBuffSize))
    {
      ESPRxBuffer[rxBufferCount++] = (char)rxChar;
          Serial.println(ESPRxBuffer[rxBufferCount-1],HEX);
      //    Serial.print(rxBufferCount);
    }
}

void clearRxBuffer()
{
  rxBufferCount = 0;
  for (uint8_t i = 0; i < ESPRxMaxBuffSize; i++)
  {
    ESPRxBuffer[i] = 0;
  }
}

boolean hal_uart_tx(char* pTxBuff, uint8_t utxCount)
{
  clearRxBuffer();
  for (uint8_t i = 0; i < utxCount; i++)
    eSerial.write(*pTxBuff++);
  return true;
}

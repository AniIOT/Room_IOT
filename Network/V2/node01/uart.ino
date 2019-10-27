#include "uart.h"
#include <NeoSWSerial.h>

#define uartBaud 38400
#define rxmaxBytes 4
#define slave_add 130
#define reqFuncCode 24

/*Software UART pins - ESP8266 pins*/
#define ESP_RX                 ((uint8_t)0x04)  //esp uart        D4
#define ESP_TX                 ((uint8_t)0x05)  //esp uart        D5

NeoSWSerial eSerial(ESP_RX, ESP_TX);

uint16_t u16Data = 0;
bool receivedFlag = false;
unsigned char rxBuffer[rxmaxBytes];

static void uart_isr(unsigned char c)
{
  static uint8_t rxCount = 0;
  rxBuffer[rxCount++] = (unsigned char)c;
  if (rxCount >= rxmaxBytes)
  {
    rxCount = 0;
    receivedFlag = true;
  }
}

void uart_init()
{
  Serial.begin(115200);
  eSerial.attachInterrupt(uart_isr);
  eSerial.begin(uartBaud); //rate could be changed
}

bool checkData()
{
  if (receivedFlag)
  {
    if (modbusReadData(rxBuffer))
    {
      receivedFlag = false;
      return true;
    }
    else
    {
      receivedFlag = false;
      return false;
    }
  }
  else
  {
    return false;
  }
}

uint16_t getData()
{
  return u16Data;
}

bool modbusReadData(unsigned char* pRxBuffer)
{
  unsigned char u8ByteRead = 0;

  u8ByteRead = *pRxBuffer++;
  if (u8ByteRead != slave_add)
  {
    memset(&pRxBuffer, 0, rxmaxBytes);
    return false;
  }

  u8ByteRead = *pRxBuffer++;
  if (u8ByteRead != reqFuncCode)
  {
    memset(&pRxBuffer, 0, rxmaxBytes);
    return false;
  }

  u16Data  = (*pRxBuffer++);
  u16Data |= (*pRxBuffer++) << 8;

  memset(&pRxBuffer, 0, rxmaxBytes);
  return true;
}

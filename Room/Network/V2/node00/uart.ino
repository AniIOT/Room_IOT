#include "uart.h"
#include <NeoSWSerial.h>

#define uartBaud 38400
#define txmaxBytes 
#define rxmaxBytes 6
#define slave_add 130
#define reqFuncCode 24

NeoSWSerial eSerial( 4, 5 );

uint16_t u16Data = 0;
bool receivedFlag = false;
unsigned char rxBuffer[rxmaxBytes];

static void uart_isr(unsigned char c)
{
  Serial.print(c);
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
    return 0;
  }

  u8ByteRead = *pRxBuffer++;
  if (u8ByteRead != reqFuncCode)
  {
    memset(&pRxBuffer, 0, rxmaxBytes);
    return 0;
  }

  u16Data = (*pRxBuffer++);
  u16Data |= (*pRxBuffer++) << 8;

  memset(&pRxBuffer, 0, rxmaxBytes);
  receivedFlag = false;
  return 1;
}

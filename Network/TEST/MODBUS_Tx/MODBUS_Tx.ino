#include <NeoSWSerial.h>

NeoSWSerial eSerial( 4, 5 );

#define uartBaud 38400
#define rxmaxBytes 4
#define slave_add 130
#define reqFuncCode 24

bool receivedFlag = false;
uint16_t u16Data = 0;
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

void setup()
{
  Serial.begin(115200);
  eSerial.attachInterrupt(uart_isr);
  eSerial.begin(uartBaud);
}

void loop()
{
  if (receivedFlag)
  {
    modbusReadData(rxBuffer);
    Serial.println((uint16_t)u16Data);
    receivedFlag = false;
  }
}

boolean modbusReadData(unsigned char* pRxBuffer)
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
  return 1;
}

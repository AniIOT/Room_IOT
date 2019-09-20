#include <NeoSWSerial.h>

NeoSWSerial eSerial( 4, 5 );

#define uartBaud 38400
#define txmaxBytes 2
#define rxmaxBytes 4
#define slave_add 130
#define reqFuncCode 24

uint16_t u16Data = 0;
uint8_t rxCount = 0;
unsigned char rxBuffer[rxmaxBytes];


static void uart_isr(unsigned char c)
{
  Serial.print(c);
  rxBuffer[rxCount] = (unsigned char)c;
  if (rxCount < rxmaxBytes)
  {
    rxCount++;
  }
  else
  {
    rxCount = 0;
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
  while (rxCount < rxmaxBytes)
  {
    delay(100);
  }
  if (modbusReadData(rxBuffer))
  {
    Serial.println(u16Data);
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

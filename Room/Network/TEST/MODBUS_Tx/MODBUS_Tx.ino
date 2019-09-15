#include <NeoSWSerial.h>

NeoSWSerial eSerial( 4, 5 );

//#include <SoftwareSerial.h>
#define uartBaud 38400
#define txmaxBytes 2
#define rxmaxBytes 4
#define slave_add 130
#define reqFuncCode 24

//SoftwareSerial eSerial(4, 5);
uint16_t u16Data = 0;
uint8_t rxCount = 0;
unsigned char rxBuffer[rxmaxBytes];


static void uart_isr(unsigned char c)
{
  rxBuffer[rxCount] = (unsigned char)c;
  rxCount++;
}

void setup()
{
  Serial.begin(115200);
  eSerial.attachInterrupt(uart_isr);
  eSerial.begin(uartBaud);
  modbusReqData();
}

void loop()
{
  while (rxCount < rxmaxBytes)
    delay(1);
  modbusReadData(rxBuffer);
  Serial.println(u16Data);
  while (1);
}

void modbusReqData()
{
  unsigned char u8TxBuffer[txmaxBytes] = {0};
  unsigned char i = 0;

  u8TxBuffer[i++] = slave_add;
  u8TxBuffer[i] = reqFuncCode;

  tx_uart(u8TxBuffer, (uint16_t)i);
}


void tx_uart(unsigned char *packet, uint16_t packetlength)
{
  uint16_t iBytes = 0;
  unsigned char dataBuff;
  unsigned char* pTxbuf = packet;

  while (iBytes <= packetlength)
  {
    //    while (eSerial.available()); //while used wait
    dataBuff = *(pTxbuf++);
    eSerial.write(dataBuff);
    iBytes++;
    eSerial.flush();
    delay(1);
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

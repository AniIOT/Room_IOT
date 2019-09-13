#include <SoftwareSerial.h>
#define uartBaud 38400
#define txmaxBytes 2
#define rxmaxBytes 4
#define slave_add 130
#define reqFuncCode 24

SoftwareSerial eSerial(4, 5);
uint16_t u16Data = 0;
uint8_t u8DataOne = 0, u8DataTwo = 0;
uint8_t checkSlaveAdd = 0, checkFunc = 0;

void setup()
{
  Serial.begin(115200);
  eSerial.begin(uartBaud);
  eSerial.flush();
  //  modbusReqData();
}

void loop()
{
  modbusReqData();
  uint8_t check = modbusReadData();
  while (!check)
  {
    check = modbusReadData();
  }
  if (check)
  {
    Serial.print(u16Data, HEX);
    Serial.print(" ");
    check = 0;
    //    while (1);

  }
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
    while (eSerial.available()); //while used wait
    dataBuff = *(pTxbuf++);
    eSerial.write(dataBuff);
    iBytes++;
    eSerial.flush();
    delay(1);
  }
}

boolean rx_uart(unsigned char** pcRxBuffer)
{
  uint8_t rxDataCount = 0;
//  while (!eSerial.available());
  if (eSerial.available() > 0)
  {
    while (rxDataCount < rxmaxBytes)
    {
      *(pcRxBuffer++) = eSerial.read();
      rxDataCount++;
    }
  }
  return 1;
}

boolean modbusReadData()
{
  boolean stat = 0;
  static unsigned char* pRxBuffer = 0;
  unsigned char u8ByteRead = 0;

  rx_uart(&pRxBuffer);

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
  
  if (u16Data < 400)
    return 1;
  else
    return 0;
}

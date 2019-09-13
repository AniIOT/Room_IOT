#define uartBaud 38400
#define txmaxBytes 4
#define rxmaxBytes 2
#define slave_add 130
#define reqFuncCode 24
#define sendData ((uint16_t)310)


void setup()
{
  Serial.begin(115200);
  Serial3.begin(uartBaud);
}

void loop()
{
  boolean check = modbusCheckReq();
  while (!check)
  {
    check = modbusCheckReq();
  }
  if (check)
  {
    delay(1);
    modbusSendData();
    check = 0;
  }
}

void modbusSendData()
{
  unsigned char u8TxBuffer[txmaxBytes] = {0};
  unsigned char i = 0;

  u8TxBuffer[i++] = slave_add;
  u8TxBuffer[i++] = reqFuncCode;
  u8TxBuffer[i++] = (unsigned char)(sendData & 0x00ff); //lsB
  u8TxBuffer[i] = (unsigned char)((sendData >> 8)); //msB
  tx_uart(u8TxBuffer, i);
}

void tx_uart(unsigned char *packet, uint16_t packetlength)
{
  uint16_t iBytes = 0;
  unsigned char dataBuff;
  unsigned char* pTxbuf = packet;

  while (iBytes <= packetlength)
  {
    while (Serial3.available()); //while used wait
    dataBuff = *(pTxbuf++);
    Serial3.write(dataBuff);
    iBytes++;
    Serial3.flush();
    delay(1);
  }
}

boolean modbusCheckReq()
{
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
  memset(&pRxBuffer, 0, rxmaxBytes);
  return 1;
}


boolean rx_uart(unsigned char** pcRxBuffer)
{
  uint8_t rxDataCount = 0;
//  while (!Serial3.available());
  if (Serial3.available() > 0)
  {
    while (rxDataCount < rxmaxBytes)
    {
      *(pcRxBuffer++) = Serial3.read();
      rxDataCount++;
    }
  }
  return 1;
}

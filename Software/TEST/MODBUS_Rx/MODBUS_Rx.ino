#define uartBaud 38400
#define txmaxBytes 4
#define rxmaxBytes 2
#define slave_add 130
#define reqFuncCode 24
#define sendData ((uint16_t)251)
boolean check = 0;

void setup()
{
  Serial.begin(115200);
  Serial3.begin(uartBaud);
}

void loop()
{
  if (Serial3.available())
  {
    check = modbusCheckReq();
  }
  if (check)
  {
    Serial.print("check");
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
    //    while (Serial3.available()); //while used wait
    dataBuff = *(pTxbuf++);
    Serial3.write(dataBuff);
    iBytes++;
    Serial3.flush();
    delay(1);
  }
}

boolean modbusCheckReq()
{
  unsigned char rxBuff[rxmaxBytes];
  unsigned char u8ByteRead = 0;

  rx_uart(rxBuff);

  u8ByteRead = rxBuff[0];
  if (u8ByteRead != slave_add)
  {
    memset(rxBuff, 0, rxmaxBytes);
    return 0;
  }

  u8ByteRead = rxBuff[1];
  if (u8ByteRead != reqFuncCode)
  {
    memset(rxBuff, 0, rxmaxBytes);
    return 0;
  }
  memset(rxBuff, 0, rxmaxBytes);
  return 1;
}


boolean rx_uart(unsigned char* pcRxBuffer)
{
  uint8_t rxDataCount = 0;
  while (rxDataCount < rxmaxBytes)
  {
    if (Serial3.available() > 0)
    {
      *pcRxBuffer++ = (Serial3.read());
      rxDataCount++;
    }
  }
  return 1;
}

void uart_init()
{
  Serial.begin(uartBaud);
}

void modbusSendData(uint16_t* pu16Data)
{
  unsigned char u8TxBuffer[txmaxBytes] = {0};
  unsigned char i = 0;
  uint16_t u16Data = *pu16Data;

  u8TxBuffer[i++] = slave_add;
  u8TxBuffer[i++] = reqFuncCode;
  u8TxBuffer[i++] = (unsigned char)(u16Data & 0x00ff); //lsB
  u8TxBuffer[i] = (unsigned char)((u16Data >> 8)); //msB
  tx_uart(u8TxBuffer, i);
}

void tx_uart(unsigned char *packet, uint16_t packetlength)
{
  uint16_t iBytes = 0;
  unsigned char dataBuff;
  unsigned char* pTxbuf = packet;

  while (iBytes <= packetlength)
  {
    dataBuff = *(pTxbuf++);
    Serial.write(dataBuff);
    iBytes++;
    Serial.flush();
    delay(1);
  }
}

boolean modbusCheckReq(unsigned char* rxBuff)
{
  if (rxCount != rxmaxBytes)
  {
    return 0;
  }
  
  unsigned char u8ByteRead = 0;

  u8ByteRead = *rxBuff++;
  if (u8ByteRead != slave_add)
  {
    memset(&rxBuff, 0, rxmaxBytes);
    return 0;
  }

  u8ByteRead = *rxBuff++;
  if (u8ByteRead != reqFuncCode)
  {
    memset(&rxBuff, 0, rxmaxBytes);
    return 0;
  }

  memset(&rxBuff, 0, rxmaxBytes);
  return 1;
}

//boolean rx_uart(unsigned char* pcRxBuffer)
//{
//  uint8_t rxDataCount = 0;
//  while (rxDataCount < rxmaxBytes)
//  {
//    if (Serial.available() > 0)
//    {
//      *pcRxBuffer++ = (Serial.read());
//      rxDataCount++;
//    }
//  }
//  return 1;
//}

void serialEvent()
{
  if (Serial.available())
  {
    RxBuffer[rxCount] = Serial.read();
    rxCount++;
  }
}

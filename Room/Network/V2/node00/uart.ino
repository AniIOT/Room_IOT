//#include "uart.h"
//
//SoftwareSerial eSerial(GPIOD_RX, GPIOD_TX);
//uint16_t u16Data = 0;
//
//void uart_init()
//{
//  eSerial.begin(uartBaud); //rate could be changed
//}
//
//void tx_uart(unsigned char *packet, uint16_t packetlength)
//{
//  uint16_t iBytes = 0;
//  unsigned char dataBuff;
//  unsigned char* pTxbuf = packet;
//
//  while (iBytes < packetlength)
//  {
//    while (eSerial.available()); //while used wait
//    dataBuff = *(pTxbuf++);
//    eSerial.write(dataBuff);
//    iBytes++;
//  }
//}
//
//boolean rx_uart(unsigned char** pcRxBuffer)
//{
//  uint8_t rxDataCount = 0;
//  if (eSerial.available() > 0)
//  {
//    while (rxDataCount < rxmaxBytes)
//    {
//      *(pcRxBuffer++) = eSerial.read();
//      rxDataCount++;
//    }
//  }
//  return 1;
//}
//
//void modbusReqData()
//{
//  unsigned char u8TxBuffer[txmaxBytes] = {0};
//  unsigned char i = 0;
//  uint16_t u16CRC = 0;
//
//  u8TxBuffer[i++] = slave_add;
//  u8TxBuffer[i] = reqFuncCode;
//
//  tx_uart(u8TxBuffer, i);
//}
//
//void modbusReadData()
//{
//  boolean stat = 0;
//  static unsigned char* pRxBuffer = 0;
//  unsigned char u8ByteRead = 0;
//
//  rx_uart(&pRxBuffer);
//
//  u8ByteRead = *pRxBuffer++;
//  if(u8ByteRead != slave_add)
//  {
//    return;
//  }
//
//  u8ByteRead = *pRxBuffer++;
//  if(u8ByteRead != reqFuncCode)
//  {
//    return;
//  }
//
//  u16Data = ((*pRxBuffer++)<<8);
//  u16Data += *pRxBuffer++;  
//}

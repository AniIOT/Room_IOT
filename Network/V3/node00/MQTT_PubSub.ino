
boolean bcheckConnectAck()
{
  unsigned char* pRxBuffer = RxBuffer;
  for (uint8_t i = 0; i < RxMaxBuffSize; i++)
  {
    if (*pRxBuffer == 0x20 && *(++pRxBuffer) == 0x02 && *(++pRxBuffer) == 0x00 && *(++pRxBuffer) == 0x00)
    {
      return true;
    }
    pRxBuffer++;
  }
  return false;
}

boolean bcheckSubscribeAck()
{
  unsigned char* pRxBuffer = RxBuffer;
  for (uint8_t i = 0; i < RxMaxBuffSize; i++)
  {
    if (*pRxBuffer == 0x90 && *(++pRxBuffer) == 0x03 && *(++pRxBuffer) == 0x00 && *(++pRxBuffer) == 0x01 && *(++pRxBuffer) == 0x00)
    {
      return true;
    }
    pRxBuffer++;
  }
  return false;
}

boolean handleMQTTresponse(boolean* pbRetry, teMQTTState eState)
{
  static uint8_t iRetryCount = 1;
  boolean checkFlag = false;

  if (eState == eMQTTConnectACK)
    checkFlag = bcheckConnectAck();
  else if (eState == eMQTTSubscribeACK)
    checkFlag = bcheckSubscribeAck();
  //  else if (eState == eMQTTPublishACK)
  //    checkFlag = ();

  if (checkFlag == true)
  {
    iRetryCount = 0;
    return true;
  }

  if (iRetryCount == 3)
  {
    iRetryCount = 0;
    *pbRetry = false;
  }
  else
  {
    *pbRetry = true;
    iRetryCount++;
  }
  return false;
}

void connectToBroker()
{
  char connectBuff[100] = {0};
  char* ptrconnectBuff = connectBuff;
  uint16_t stringLen = 0;
  uint8_t charCounter = 0;

  /*Fixed header*/
  *ptrconnectBuff++ = (MQTT_CTRL_CONNECT | MQTT_CTRL_CONNECT_FLAG);
  charCounter++;
  ptrconnectBuff++;
  charCounter++;

  /*Variable Header*/
  //Protocol name
  *ptrconnectBuff++ = 0x00;
  *ptrconnectBuff++ = 0x04;
  *ptrconnectBuff++ = 'M';
  *ptrconnectBuff++ = 'Q';
  *ptrconnectBuff++ = 'T';
  *ptrconnectBuff++ = 'T';
  charCounter += 6;

  //Protocol Level
  *ptrconnectBuff++ = MQTT_PROTOCOL_LEVEL;
  charCounter++;
  //Connect Flags
  *ptrconnectBuff++ = 0xC2; //Username Flag | Password Flag | Will Retain | Will QoS | Will Flag | Clean Session | Reserved
  charCounter++;
  //Keep alive time
  *ptrconnectBuff++ = (char)(0xFF00 & MQTT_CONN_KEEPALIVE);
  *ptrconnectBuff++ = (char)(0x00FF & MQTT_CONN_KEEPALIVE);
  charCounter += 2;

  /*Payload*/
  //CID length
  *ptrconnectBuff++ = 0x00;
  *ptrconnectBuff++ = 0x06;
  charCounter += 2;

  //CID
  *ptrconnectBuff++ = 'A';
  *ptrconnectBuff++ = 'B';
  *ptrconnectBuff++ = 'C';
  *ptrconnectBuff++ = 'D';
  *ptrconnectBuff++ = 'E';
  *ptrconnectBuff++ = 'F';
  charCounter += 6;

  //UID length
  stringLen = (uint16_t)strlen(MQTT_USERNAME);
  *ptrconnectBuff++ = (char)(0xFF00 & stringLen);
  *ptrconnectBuff++ = (char)(0x00FF & stringLen);
  charCounter += 2;

  //UID
  memcpy(ptrconnectBuff, MQTT_USERNAME, sizeof(MQTT_USERNAME));
  ptrconnectBuff += stringLen;
  charCounter += stringLen;

  //Password length
  stringLen = (uint16_t)strlen(MQTT_PASSWORD);
  *ptrconnectBuff++ = (char)(0xFF00 & stringLen);
  *ptrconnectBuff++ = (char)(0x00FF & stringLen);
  charCounter += 2;

  //Password
  memcpy(ptrconnectBuff, MQTT_PASSWORD, sizeof(MQTT_PASSWORD));
  charCounter += stringLen;

  /*Add Remaining Length*/
  connectBuff[1] = charCounter - 2;

  hal_uart_tx(connectBuff, charCounter);
  //
  //  for (int i = 0; i < 62; i++)
  //    Serial.println(connectBuff[i], HEX);
}

void subscribeToTopic(char * ptrTopic)
{
  volatile unsigned char subscribeBuff[20];
  memset(subscribeBuff, 0, 20 * sizeof(unsigned char));
  unsigned char* ptrsubscribeBuff = subscribeBuff;
  uint16_t stringLen = 0;
  uint8_t charCounter = 0;

  /*Fixed header*/
  *ptrsubscribeBuff++ = ( MQTT_CTRL_SUBSCRIBE | MQTT_CTRL_SUBSCRIBE_FLAG );
  charCounter++;
  ptrsubscribeBuff++;
  charCounter++;

  /*Variable Header*/
  //Packet ID
  *ptrsubscribeBuff++ = 0x00;
  *ptrsubscribeBuff++ = 0x01;
  charCounter += 2;

  /*Payload*/
  //Topic length
  stringLen = (uint16_t)strlen(MQTT_TOPIC);
  *ptrsubscribeBuff++ = (char)(0xFF00 & stringLen);
  *ptrsubscribeBuff++ = (char)(0x00FF & stringLen);
  charCounter += 2;

  //Topic
  for (uint8_t i = 0; i < stringLen; i++)
    *ptrsubscribeBuff++ = *ptrTopic++;
  charCounter += stringLen;

  //QoS
  *ptrsubscribeBuff = MQTT_QOS_0;
  charCounter ++
  ;

  //Add remaining length
  subscribeBuff[1] = charCounter - 2;

  hal_uart_tx(subscribeBuff, charCounter);

  //  for (int i = 0; i < charCounter; i++)
  //    Serial.println(subscribeBuff[i], HEX);
}

void publishToTopic(char * ptrTopic, char* ptrData)
{
  volatile unsigned char publishBuff[30];
  memset(publishBuff, 0, 30 * sizeof(unsigned char));
  unsigned char* ptrpublishBuff = publishBuff;
  uint16_t stringLen = 0;
  uint8_t charCounter = 0;

  /*Fixed header*/
  *ptrpublishBuff++ = ( MQTT_CTRL_PUBLISH | MQTT_CTRL_PUBLISH_FLAG | MQTT_RETAIN);
  charCounter++;
  ptrpublishBuff++;
  charCounter++;

  /*Variable Header*/
  //Topic length
  stringLen = (uint16_t)strlen(MQTT_TOPIC);
  *ptrpublishBuff++ = (char)(0xFF00 & stringLen);
  *ptrpublishBuff++ = (char)(0x00FF & stringLen);
  charCounter += 2;

  //Topic
  for (uint8_t i = 0; i < stringLen; i++)
    *ptrpublishBuff++ = *ptrTopic++;
  charCounter += stringLen;

  //Packet ID
  *ptrpublishBuff++ = 0x00;
  *ptrpublishBuff++ = 0x01;
  charCounter += 2;

  /*Payload*/
  //Data

  //Add remaining length
  publishBuff[1] = charCounter - 2;

  hal_uart_tx(publishBuff, charCounter);

  //  for (int i = 0; i < charCounter; i++)
  //    Serial.println(subscribeBuff[i], HEX);
}

void pingToServer()
{
  char pingBuff[2] = {0, 0};
  char* ptrpingBuff = pingBuff;

  *ptrpingBuff++ = ( MQTT_CTRL_PINGREQ | MQTT_CTRL_PINGREQ_FLAG );
  *ptrpingBuff = 0x00;                                              //Remaining length is zero

  Serial.println(".");
  hal_uart_tx(pingBuff, 2);
}

teMQTTstatus MQTTStateMachine()
{
  static teMQTTstatus eMQTTStatus = eMQTTInProgress;
  static teMQTTState eMQTTstate = eMQTTInit;
  static boolean bRetry = true;

  switch (eMQTTstate)
  {
    case eMQTTInit:
      if (ESPStateMachine() == eEspSuccess)
        eMQTTstate = eMQTTConnectReq;
      break;

    case eMQTTConnectReq:
      connectToBroker();
      eMQTTstate = eMQTTConnectACK;
      Serial.println("Connecting to AdafruitIO MQTT Broker");
      break;

    case eMQTTConnectACK:
      delay(500);
      if (handleMQTTresponse(&bRetry, eMQTTConnectACK) == true)
      {
        eMQTTstate = eMQTTSubscribeReq;
        Serial.println("Connection to AdafruitIO MQTT Broker Successful");
      }
      else if (bRetry == true)
      {
        eMQTTstate = eMQTTConnectReq;
      }
      else
      {
        //will be decided later
        //possibly recall esp state machine
      }

      break;

    case eMQTTSubscribeReq:
      subscribeToTopic((char*)MQTT_TOPIC);
      eMQTTstate = eMQTTSubscribeACK;
      Serial.println("Subscribing to topic ""\""MQTT_TOPIC"\"");
      break;

    case eMQTTSubscribeACK:
      delay(1000);
      if (handleMQTTresponse(&bRetry, eMQTTSubscribeACK) == true)
      {
        eMQTTstate = eMQTTSuccessState;
        Serial.println("Subscription to "MQTT_TOPIC" was successful");
      }
      else if (bRetry == true)
      {
        eMQTTstate = eMQTTSubscribeReq;
      }
      else
      {
        Serial.println("Check code");
        while (1);
        //will be decided later
        //possibly recall esp state machine
      }
      break;

    case eMQTTPublishReq:
      break;

    case eMQTTPublishACK:
      break;

    case eMQTTSuccessState:
      eMQTTStatus = eMQTTSuccess;
      break;

    case eMQTTErrorState:
      break;
  }
  return eMQTTStatus;
}

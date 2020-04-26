
static teMQTTstatus eMQTTStatus = eMQTTInProgress;
static teMQTTState eMQTTstate = eMQTTInit;

void resetMQTT()
{
  MQTTInitFlag = false;
  eMQTTstate = eMQTTInit;
  eMQTTStatus = eMQTTInProgress;
}
boolean bcheckConnectAck()
{
  unsigned char* pRxBuffer = RxBuffer;
  for (uint8_t i = 0; i < RxMaxBuffSize; i++)
  {
    if (*pRxBuffer == MQTT_CTRL_CONNECTACK && *(++pRxBuffer) == (rxBufferCount - 2) && *(++pRxBuffer) == 0x00 && *(++pRxBuffer) == MQTT_POS_ACK)
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
    if (*pRxBuffer == MQTT_CTRL_SUBACK && *(++pRxBuffer) == (rxBufferCount - 2) && *(++pRxBuffer) == (SubPacketID & 0xFF00) && *(++pRxBuffer) == (SubPacketID & 0x00FF) && *(++pRxBuffer) == MQTT_POS_ACK)
    {
      if (SubPacketID == 0xFFFF)
        SubPacketID = 0;
      else
        SubPacketID++;
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

void connectToBroker(uint8_t u8connectFlags, unsigned char* ucClientID, unsigned char* ucUsername, unsigned char* ucPassword, uint16_t u16KeepAliveTimeS)
{
  unsigned char connectBuff[100] = {0}; //TODO: decide buffer length as username and password can vary and can be very long
  unsigned char* ptrconnectBuff = connectBuff;
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
  *ptrconnectBuff++ = 'T'; //This will stay same unless protocol level changes, can be also made generic by having protocol level and name as parameters
  charCounter += 6;
  //Protocol Level
  *ptrconnectBuff++ = MQTT_PROTOCOL_LEVEL;
  charCounter++;
  //Connect Flags
  *ptrconnectBuff++ = (unsigned char)u8connectFlags;//Username Flag | Password Flag | Will Retain | Will QoS | Will Flag | Clean Session | Reserved
  charCounter++;
  //Keep alive time
  *ptrconnectBuff++ = (unsigned char)(0xFF00 & u16KeepAliveTimeS);
  *ptrconnectBuff++ = (unsigned char)(0x00FF & u16KeepAliveTimeS);
  charCounter += 2;

  /*Payload*/
  //CID length
  stringLen = (uint16_t)strlen(ucClientID);
  *ptrconnectBuff++ = (stringLen & 0xFF00);
  *ptrconnectBuff++ = (stringLen & 0x00FF);
  charCounter += 2;
  if (stringLen > 0)
  {
    //CID
    for (uint8_t i = 0; i < (uint8_t)stringLen; i++)
      *ptrconnectBuff++ = *ucClientID++;
    charCounter += stringLen;
  }
  //UID length
  stringLen = (uint16_t)strlen(ucUsername);
  *ptrconnectBuff++ = (char)(0xFF00 & stringLen);
  *ptrconnectBuff++ = (char)(0x00FF & stringLen);
  charCounter += 2;
  //UID
  for (uint8_t i = 0; i < (uint8_t)stringLen; i++)
    *ptrconnectBuff++ = *ucUsername++;
  charCounter += stringLen;
  //Password length
  stringLen = (uint16_t)strlen(ucPassword);
  *ptrconnectBuff++ = (char)(0xFF00 & stringLen);
  *ptrconnectBuff++ = (char)(0x00FF & stringLen);
  charCounter += 2;
  //Password
  for (uint8_t i = 0; i < (uint8_t)stringLen; i++)
    *ptrconnectBuff++ = *ucPassword++;
  charCounter += stringLen;

  /*Add Remaining Length*/
  connectBuff[1] = charCounter - 2;

  /*Send data through uart to server*/
  hal_uart_tx(connectBuff, charCounter);

  //  for (int i = 0; i < charCounter; i++)
  //    Serial.println(connectBuff[i], HEX);
}

void subscribeToTopic(char* ptrTopic, uint8_t uiQoS)
{
  unsigned char subscribeBuff[200]; //TODO: check if this length is enough by considering a long topic name
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
  *ptrsubscribeBuff++ = (SubPacketID & 0xFF00);
  *ptrsubscribeBuff++ = (SubPacketID & 0x00FF);
  charCounter += 2;

  /*Payload*/
  //Topic length
  stringLen = (uint16_t)strlen(ptrTopic);
  *ptrsubscribeBuff++ = (char)(0xFF00 & stringLen);
  *ptrsubscribeBuff++ = (char)(0x00FF & stringLen);
  charCounter += 2;
  //Topic
  for (uint8_t i = 0; i < stringLen; i++)
    *ptrsubscribeBuff++ = *ptrTopic++;
  charCounter += stringLen;
  //QoS
  *ptrsubscribeBuff++ = uiQoS;
  charCounter++;

  /*Add remaining length*/
  subscribeBuff[1] = charCounter - 2;

  /*Send data through uart to server*/
  hal_uart_tx(subscribeBuff, charCounter);

  //  for (int i = 0; i < charCounter; i++)
  //    Serial.println(subscribeBuff[i], HEX);
}

void publishToTopic(char * ptrTopic, char* ptrData, uint8_t uiQoS, uint8_t u8RetainFlag)
{
  unsigned char publishBuff[30];
  unsigned char* ptrpublishBuff = publishBuff;
  uint16_t stringLen = 0;
  uint8_t charCounter = 0;

  /*Fixed header*/
  *ptrpublishBuff++ = ( MQTT_CTRL_PUBLISH | MQTT_CTRL_PUBLISH_FLAG | u8RetainFlag | (uiQoS < 1));
  charCounter++;
  ptrpublishBuff++;
  charCounter++;

  /*Variable Header*/
  //Topic length
  stringLen = (uint16_t)strlen(ptrTopic);
  *ptrpublishBuff++ = (char)(0xFF00 & stringLen);
  *ptrpublishBuff++ = (char)(0x00FF & stringLen);
  charCounter += 2;
  //Topic
  for (uint8_t i = 0; i < stringLen; i++)
    *ptrpublishBuff++ = *ptrTopic++;
  charCounter += stringLen;
  //Packet ID
  //  *ptrpublishBuff++ = (PubPacketID & 0xFF00);
  //  *ptrpublishBuff++ = (PubPacketID & 0x00FF);
  //  charCounter += 2;

  /*Payload*/
  //Data
  stringLen = (uint16_t)strlen(ptrData);
  for (uint8_t i = 0; i < stringLen; i++)
    *ptrpublishBuff++ = *ptrData++;
  charCounter += stringLen;

  /*Add remaining length*/
  publishBuff[1] = charCounter - 2;

  /*Send data through uart to server*/
  hal_uart_tx(publishBuff, charCounter);

  //    for (int i = 0; i < charCounter; i++)
  //      Serial.println(publishBuff[i], HEX);
}

void pingToServer()
{
  char pingBuff[2] = {0, 0};
  char* ptrpingBuff = pingBuff;

  *ptrpingBuff++ = ( MQTT_CTRL_PINGREQ | MQTT_CTRL_PINGREQ_FLAG );
  *ptrpingBuff = 0x00;                                              //Remaining length is zero
  if (u8pingCount > 2)
  {
    resetMQTT();
    resetESP();
    u8pingCount = 0;
    return;
  }
  u8pingCount += 2;
  //  Serial.print(F(".\r\n"));
  hal_uart_tx(pingBuff, 2);
}

void publishIfMoodReq()
{
  if (MoodAOff)
  {
    publishToTopic(MQTT_TOPIC2, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC3, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC4, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC5, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC6, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC7, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC8, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC10, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC11, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC12, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC13, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC14, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC15, "OFF", MQTT_QOS_0, MQTT_RETAIN);
    MoodAOff = false;
  }
  if (MoodChill)
  {
    publishToTopic(MQTT_TOPIC3, "ON", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC4, "ON", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC5, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC6, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC7, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC8, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC10, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC11, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC13, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC14, "OFF", MQTT_QOS_0, MQTT_RETAIN);
    MoodChill = false;
  }
  if (MoodWork)
  {
    publishToTopic(MQTT_TOPIC3, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC4, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC5, "ON", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC6, "ON", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC7, "ON", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC8, "ON", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC10, "ON", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC11, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC13, "OFF", MQTT_QOS_0, MQTT_RETAIN);    delay(2);
    publishToTopic(MQTT_TOPIC14, "OFF", MQTT_QOS_0, MQTT_RETAIN);
    MoodWork = false;
  }
}

teMQTTstatus MQTTStateMachine()
{
  static boolean bRetry = true;

  switch (eMQTTstate)
  {
    case eMQTTInit:
      if (ESPStateMachine() == eEspSuccess)
        eMQTTstate = eMQTTConnectReq;
      break;

    case eMQTTConnectReq:
      connectToBroker((MQTT_CONN_USERNAMEFLAG | MQTT_CONN_PASSWORDFLAG | MQTT_CONN_CLEANSESSION), "Aniruddha", MQTT_USERNAME, MQTT_PASSWORD, MQTT_CONN_KEEPALIVE);
      eMQTTstate = eMQTTConnectACK;
      Serial.print(F("Connecting to AdafruitIO MQTT Broker\r\n"));
      break;

    case eMQTTConnectACK:
      delay(2000);
      if (handleMQTTresponse(&bRetry, eMQTTConnectACK) == true)
      {
        eMQTTstate = eMQTTSubscribeReq;
        Serial.print(F("Connection Successful\r\n"));
      }
      else if (bRetry == true)
      {
        eMQTTstate = eMQTTConnectReq;
      }
      else
      {
        resetMQTT();
        resetESP();
        //will be decided later
        //possibly recall esp state machine
      }

      break;

    case eMQTTSubscribeReq:
      Serial.print(F("Subscribing to topics\r\n"));
      subscribeToTopic((char*)MQTT_TOPIC1, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC2, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC3, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC4, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC5, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC6, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC7, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC8, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC9, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC10, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC11, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC12, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC13, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC14, MQTT_QOS_0);
      delay(500);
      subscribeToTopic((char*)MQTT_TOPIC15, MQTT_QOS_0);
      eMQTTstate = eMQTTSubscribeACK;
      break;

    case eMQTTSubscribeACK:
      delay(4000);
      if (handleMQTTresponse(&bRetry, eMQTTSubscribeACK) == true)
      {
        eMQTTstate = eMQTTSuccessState;
        Serial.print(F("Subscription to all topics was successful\r\n"));
      }
      else if (bRetry == true)
      {
        eMQTTstate = eMQTTSubscribeReq;
      }
      else
      {
        resetMQTT();
        resetESP();
        Serial.print(F("Check code\r\n"));
        //        while (1);
        //will be decided later
        //possibly recall esp state machine
      }
      break;

    case eMQTTPublishReq:
      //      publishToTopic(MQTT_TOPIC, "111", MQTT_QOS_0, MQTT_RETAIN);
      eMQTTstate = eMQTTPublishACK;
      break;

    case eMQTTPublishACK:
      break;

    case eMQTTSuccessState:
      MQTTInitFlag = true;
      eMQTTStatus = eMQTTSuccess;
      break;

    case eMQTTErrorState:
      break;
  }
  return eMQTTStatus;
}

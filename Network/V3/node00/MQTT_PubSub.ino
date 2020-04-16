boolean handleMQTTresponse(boolean bRetry, teMQTTState eState)
{

}
void connectToBroker()
{
  char connectBuff[100] = {0};
  char* ptrconnectBuff = connectBuff;
  
  /*Fixed header*/
  *ptrconnectBuff++ = MQTT_CTRL_CONNECT_FLAG | MQTT_CTRL_CONNECT << 4;
  *ptrconnectBuff++ = 0x3C; //Add length later

  /*Variable Header*/
  //Protocol name
  *ptrconnectBuff++ = 0x00;
  *ptrconnectBuff++ = 0x04;
  *ptrconnectBuff++ = 'M';
  *ptrconnectBuff++ = 'Q';
  *ptrconnectBuff++ = 'T';
  *ptrconnectBuff++ = 'T';
  //Protocol Level
  *ptrconnectBuff++ = MQTT_PROTOCOL_LEVEL;
  //Connect Flags
  *ptrconnectBuff++ = 0xC2; //Username Flag | Password Flag | Will Retain | Will QoS | Will Flag | Clean Session | Reserved
  //Keep alive time
  *ptrconnectBuff++ = (char)(0xFF00 & MQTT_CONN_KEEPALIVE);
  *ptrconnectBuff++ = (char)(0x00FF & MQTT_CONN_KEEPALIVE);

  /*Payload*/
  //CID length
  *ptrconnectBuff++ = 0x00;
  *ptrconnectBuff++ = 0x06;
  
  //CID
  *ptrconnectBuff++ = 'A';
  *ptrconnectBuff++ = 'B';
  *ptrconnectBuff++ = 'C';
  *ptrconnectBuff++ = 'D';
  *ptrconnectBuff++ = 'E';
  *ptrconnectBuff++ = 'F';

  //UID length
  *ptrconnectBuff++ = (char)(0xFF00 & (uint16_t)strlen(MQTT_USERNAME));
  *ptrconnectBuff++ = (char)(0x00FF & (uint16_t)strlen(MQTT_USERNAME));

  //UID
  memcpy(ptrconnectBuff, MQTT_USERNAME, sizeof(MQTT_USERNAME));

  ptrconnectBuff+= (uint16_t)strlen(MQTT_USERNAME);

  //Password length
  *ptrconnectBuff++ = (char)(0xFF00 & (uint16_t)strlen(MQTT_PASSWORD));
  *ptrconnectBuff++ = (char)(0x00FF & (uint16_t)strlen(MQTT_PASSWORD));

  //Password
  memcpy(ptrconnectBuff, MQTT_PASSWORD, sizeof(MQTT_PASSWORD));

  
  hal_uart_tx(connectBuff, 62);
//  
//  for (int i = 0; i < 62; i++)
//    Serial.println(connectBuff[i], HEX);
}

teMQTTstatus MQTTStateMachine()
{
  static teMQTTstatus eMQTTStatus = eMQTTInProgress;
  static teMQTTState eMQTTstate = eMQTTConnectReq;
  static boolean bRetry = true;

  switch (eMQTTstate)
  {
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
      break;

    case eMQTTSubscribeACK:
      break;

    case eMQTTPublishReq:
      break;

    case eMQTTPublishACK:
      break;
  }
  return eMQTTStatus;
}

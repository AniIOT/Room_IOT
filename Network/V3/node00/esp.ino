

boolean bcheckOKresp()
{
  char* pRxBuffer = ESPRxBuffer;
  for (uint8_t i = 0; i < ESPRxMaxBuffSize; i++)
  {
    if (*pRxBuffer == 'O' && *(++pRxBuffer) == 'K')
    {
      return true;
    }
    pRxBuffer++;
  }
  return false;

}

boolean bcheckCONNECTresp()
{
  char* pRxBuffer = ESPRxBuffer;
  for (uint8_t i = 0; i < ESPRxMaxBuffSize; i++)
  {
    if (*pRxBuffer == 'C' && *(++pRxBuffer) == 'O' && *(++pRxBuffer) == 'N' && *(++pRxBuffer) == 'N' \
        && *(++pRxBuffer) == 'E' && *(++pRxBuffer) == 'C' && *(++pRxBuffer) == 'T')
    {
      return true;
    }
    pRxBuffer++;
  }
  return false;

}

boolean bcheckSENDresp()
{
  char* pRxBuffer = ESPRxBuffer;
  for (uint8_t i = 0; i < ESPRxMaxBuffSize; i++)
  {
    if (*pRxBuffer == '>')
    {
      return true;
    }
    pRxBuffer++;
  }
  return false;

}

boolean handleRespose(boolean* pbRetry, teESPstate ehandleESPstate)
{
  static uint8_t iRetryCount = 1;
  boolean checkFlag = false;

  if (ehandleESPstate == eEspSetupTCPResp)
    checkFlag = bcheckCONNECTresp();
  else if (ehandleESPstate == eEspSendResp)
    checkFlag = bcheckSENDresp();
  else
    checkFlag = bcheckOKresp();
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

teESPstatus ESPStateMachine()
{
  static teESPstate eESPstate = eEspATReq;
  static boolean bRetry = true;
  static teESPstatus teStatus = eEspInProgress;

  switch (eESPstate)
  {
    case eEspATReq:
      teStatus = eEspInProgress;
      hal_uart_tx((char*)("AT\r\n"), 4);
      eESPstate = eEspATResp;
      Serial.print("ESP-REQ-AT\r\n");
      break;

    case eEspATResp:
      delay(100);
      if (handleRespose(&bRetry , eEspATResp) == true)
      {
        eESPstate = eEspATE0Req;
        Serial.print("ESP-RESP-AT\r\n");
      }
      else if (bRetry == true)
      {
        eESPstate = eEspATReq;
      }
      else
      {
        Serial.print("Check connection with ESP\r\n"); //print error if even after set number of retries ESP doesn't respond
      }
      break;

    case eEspATE0Req:
      hal_uart_tx((char*)("ATE0\r\n"), 6);
      eESPstate = eEspATE0Resp;
      Serial.print("ESP-REQ-ATE0\r\n");
      break;

    case eEspATE0Resp:
      delay(100);
      if (handleRespose(&bRetry , eEspATE0Resp) == true)
      {
        eESPstate = eEspStationModeReq;
        Serial.print("ESP-RESP-ATE0\r\n");
      }
      else if (bRetry == true)
      {
        eESPstate = eEspATE0Req;
      }
      else
      {
        eESPstate = eEspATReq;
        Serial.print("Check connection with ESP\r\n"); //print error if even after set number of retries ESP doesn't respond
      }
      break;

    case eEspStationModeReq:
      hal_uart_tx((char*)("AT+CWMODE=1\r\n"), 13);
      eESPstate = eEspStationModeResp;
      Serial.print("ESP-REQ-STA\r\n");
      break;

    case eEspStationModeResp:
      delay(100);
      if (handleRespose(&bRetry , eEspStationModeResp) == true)
      {
        eESPstate = eEspConnectToAccessPointReq;
        Serial.print("ESP-RESP-STA\r\n");
      }
      else if (bRetry == true)
      {
        eESPstate = eEspStationModeReq;
      }
      else
      {
        eESPstate = eEspATReq;
        Serial.print("Check connection with ESP\r\n"); //print error if even after set number of retries ESP doesn't respond
      }
      break;

    case eEspConnectToAccessPointReq:
      hal_uart_tx((char*)("AT+CWJAP=\"Damle 3rd Floor\",\"A342320D\"\r\n"), 39);
      eESPstate = eEspConnectToAccessPointResp;
      Serial.print("ESP-REQ-CON\r\n");
      break;

    case eEspConnectToAccessPointResp:
      delay(7500);
      if (handleRespose(&bRetry , eEspConnectToAccessPointResp) == true)
      {
        eESPstate = eEspSingleConnModeReq;
        Serial.print("ESP-RESP-CON\r\n");
      }
      else if (bRetry == true)
      {
        eESPstate = eEspConnectToAccessPointReq;
      }
      else
      {
        eESPstate = eEspATReq;
        Serial.print("Check connection with ESP\r\n"); //print error if even after set number of retries ESP doesn't respond
      }
      break;

    case eEspSingleConnModeReq:
      hal_uart_tx((char*)("AT+CIPMUX=0\r\n"), 13);
      eESPstate = eEspSingleConnModeResp;
      Serial.print("ESP-REQ-MUX\r\n");
      break;

    case eEspSingleConnModeResp:
      delay(100);
      if (handleRespose(&bRetry , eEspSingleConnModeResp) == true)
      {
        eESPstate = eEspTransparentModeOffReq;
        Serial.print("ESP-RESP-MUX\r\n");
      }
      else if (bRetry == true)
      {
        eESPstate = eEspSingleConnModeReq;
      }
      else
      {
        eESPstate = eEspATReq;
        Serial.print("Check connection with ESP\r\n"); //print error if even after set number of retries ESP doesn't respond
      }
      break;

    case eEspTransparentModeOffReq:
      hal_uart_tx((char*)("AT+CIPMODE=0\r\n"), 14);
      eESPstate = eEspTransparentModeOffResp;
      Serial.print("ESP-REQ-PASS-OFF\r\n");
      break;

    case eEspTransparentModeOffResp:
      delay(100);
      if (handleRespose(&bRetry , eEspTransparentModeOffResp) == true)
      {
        eESPstate = eEspSetupTCPReq;
        Serial.print("ESP-RESP-PASS-OFF\r\n");
      }
      else if (bRetry == true)
      {
        eESPstate = eEspTransparentModeOffReq;
      }
      else
      {
        eESPstate = eEspATReq;
        Serial.print("Check connection with ESP\r\n"); //print error if even after set number of retries ESP doesn't respond
      }
      break;

    case eEspSetupTCPReq:
      hal_uart_tx((char*)("AT+CIPSTART=\"TCP\",\"io.adafruit.com\",1883\r\n"), 42);
      eESPstate = eEspSetupTCPResp;
      Serial.print("ESP-REQ-TCP\r\n");
      break;

    case eEspSetupTCPResp:
      delay(100);
      if (handleRespose(&bRetry , eEspSetupTCPResp) == true)
      {
        eESPstate = eEspTransparentModeOnReq;
        Serial.print("ESP-RESP-TCP\r\n");
      }
      else if (bRetry == true)
      {
        eESPstate = eEspSetupTCPReq;
      }
      else
      {
        eESPstate = eEspATReq;
        Serial.print("Check connection with ESP\r\n"); //print error if even after set number of retries ESP doesn't respond
      }
      break;

    case eEspTransparentModeOnReq:
      hal_uart_tx((char*)("AT+CIPMODE=1\r\n"), 14);
      eESPstate = eEspTransparentModeOnResp;
      Serial.print("ESP-REQ-PASS-ON\r\n");
      break;

    case eEspTransparentModeOnResp:
      delay(100);
      if (handleRespose(&bRetry , eEspTransparentModeOnResp) == true)
      {
        eESPstate = eEspSendReq;
        Serial.print("ESP-RESP-PASS-ON\r\n");
      }
      else if (bRetry == true)
      {
        eESPstate = eEspTransparentModeOnReq;
      }
      else
      {
        eESPstate = eEspATReq;
        Serial.print("Check connection with ESP\r\n"); //print error if even after set number of retries ESP doesn't respond
      }
      break;

    case eEspSendReq:
      hal_uart_tx((char*)("AT+CIPSEND\r\n"), 12);
      eESPstate = eEspSendResp;
      Serial.print("ESP-REQ-SEND\r\n");
      break;

    case eEspSendResp:
      delay(100);
      if (handleRespose(&bRetry , eEspSendResp) == true)
      {
        eESPstate = eEspSuccessState;
        Serial.print("ESP-RESP-SEND\r\n");
      }
      else if (bRetry == true)
      {
        eESPstate = eEspSendReq;
      }
      else
      {
        eESPstate = eEspATReq;
        Serial.print("Check connection with ESP\r\n"); //print error if even after set number of retries ESP doesn't respond
      }
      break;

    case eEspexittransparentmode:
      hal_uart_tx((char*)("+++"), 3);
      eESPstate = eEspATReq;
      Serial.print("ESP-REQ-EXT\r\n");
      break;

    case eEspSuccessState:
      teStatus = eEspSuccess;
      break;

    case eEspErrorState:
      teStatus = eEspError;
      break;

    default:
      break;
  }
  return teStatus;
}

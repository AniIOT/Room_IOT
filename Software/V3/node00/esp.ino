
static teESPstate eESPstate = eEspexittransparentmodeReq;
static teESPstatus eESPStatus = eEspInProgress;

void resetESP()
{
  ESPInitFlag = false;
  eESPstate = eEspexittransparentmodeReq;
  eESPStatus = eEspInProgress;
}

boolean bcheckOKresp()
{
  char* pRxBuffer = RxBuffer;
  for (uint8_t i = 0; i < RxMaxBuffSize; i++)
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
  char* pRxBuffer = RxBuffer;
  for (uint8_t i = 0; i < RxMaxBuffSize; i++)
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
  char* pRxBuffer = RxBuffer;
  for (uint8_t i = 0; i < RxMaxBuffSize; i++)
  {
    if (*pRxBuffer == '>')
    {
      return true;
    }
    pRxBuffer++;
  }
  return false;
}

boolean bcheckEXTresp()
{
  char* pRxBuffer = RxBuffer;
  for (uint8_t i = 0; i < RxMaxBuffSize; i++)
  {
    if (*pRxBuffer == 'C' && *(++pRxBuffer) == 'L' && *(++pRxBuffer) == 'O' && *(++pRxBuffer) == 'S' \
        && *(++pRxBuffer) == 'E' && *(++pRxBuffer) == 'D')
    {
      return true;
    }
    pRxBuffer++;
  }
  return false;
}

boolean bcheckConnectionresp()
{
  char* pRxBuffer = RxBuffer;
  for (uint8_t i = 0; i < RxMaxBuffSize; i++)
  {
    if (*pRxBuffer == 'D' && *(++pRxBuffer) == 'a' && *(++pRxBuffer) == 'm' && *(++pRxBuffer) == 'l' && *(++pRxBuffer) == 'e')
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
  else if (ehandleESPstate == eEspexittransparentmodeResp)
    checkFlag = bcheckEXTresp();
  else if (ehandleESPstate == eEspCheckConnectionToAccessPointResp)
    checkFlag = bcheckConnectionresp();
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
  static boolean bRetry = true;
  static uint8_t iTime100ms = 0;

  switch (eESPstate)
  {
    case eEspATReq:
      eESPStatus = eEspInProgress;
      hal_uart_tx((char*)("AT\r\n"), 4);
      eESPstate = eEspATResp;
      Serial.print(F("ESP-REQ-AT\r\n"));
      break;

    case eEspATResp:
      if (iTime100ms >= 1)
      {
        if (handleRespose(&bRetry , eEspATResp) == true)
        {
          eESPstate = eEspATE0Req;
          Serial.print(F("ESP-RESP-AT\r\n"));
        }
        else if (bRetry == true)
        {
          eESPstate = eEspATReq;
        }
        else
        {
          Serial.print(F("Check connection with ESP\r\n")); //print error if even after set number of retries ESP doesn't respond
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspATE0Req:
      hal_uart_tx((char*)("ATE0\r\n"), 6);
      eESPstate = eEspATE0Resp;
      Serial.print(F("ESP-REQ-ATE0\r\n"));
      break;

    case eEspATE0Resp:
      if (iTime100ms >= 1)
      {
        if (handleRespose(&bRetry , eEspATE0Resp) == true)
        {
          eESPstate = eEspStationModeReq;
          Serial.print(F("ESP-RESP-ATE0\r\n"));
        }
        else if (bRetry == true)
        {
          eESPstate = eEspATE0Req;
        }
        else
        {
          eESPstate = eEspATReq;
          Serial.print(F("Check connection with ESP\r\n")); //print error if even after set number of retries ESP doesn't respond
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspStationModeReq:
      hal_uart_tx((char*)("AT+CWMODE=1\r\n"), 13);
      eESPstate = eEspStationModeResp;
      Serial.print(F("ESP-REQ-STA\r\n"));
      break;

    case eEspStationModeResp:
      if (iTime100ms >= 1)
      {
        if (handleRespose(&bRetry , eEspStationModeResp) == true)
        {
          eESPstate = eEspCheckConnectionToAccessPointReq;
          Serial.print(F("ESP-RESP-STA\r\n"));
        }
        else if (bRetry == true)
        {
          eESPstate = eEspStationModeReq;
        }
        else
        {
          eESPstate = eEspATReq;
          Serial.print(F("Check connection with ESP\r\n")); //print error if even after set number of retries ESP doesn't respond
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspCheckConnectionToAccessPointReq:
      hal_uart_tx((char*)("AT+CWJAP?\r\n"), 11);
      eESPstate = eEspCheckConnectionToAccessPointResp;
      Serial.print(F("ESP-REQ-CHE\r\n"));
      break;

    case eEspCheckConnectionToAccessPointResp:
      if (iTime100ms >= 1)
      {
        if (handleRespose(&bRetry , eEspCheckConnectionToAccessPointResp) == true)
        {
          eESPstate = eEspTransparentModeOffReq;
          Serial.print(F("ESP-RESP-CHE\r\n"));
        }
        else
        {
          eESPstate = eEspConnectToAccessPointReq;
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspConnectToAccessPointReq:
      hal_uart_tx((char*)("AT+CWJAP=\"Damle 3rd Floor\",\"A342320D\"\r\n"), 39);
      eESPstate = eEspConnectToAccessPointResp;
      Serial.print(F("ESP-REQ-CON\r\n"));
      break;

    case eEspConnectToAccessPointResp:
      if (iTime100ms >= 75)
      {
        if (handleRespose(&bRetry , eEspConnectToAccessPointResp) == true)
        {
          eESPstate = eEspSingleConnModeReq;
          Serial.print(F("ESP-RESP-CON\r\n"));
        }
        else if (bRetry == true)
        {
          eESPstate = eEspConnectToAccessPointReq;
        }
        else
        {
          eESPstate = eEspATReq;
          Serial.print(F("Check connection with ESP\r\n")); //print error if even after set number of retries ESP doesn't respond
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspSingleConnModeReq:
      hal_uart_tx((char*)("AT+CIPMUX=0\r\n"), 13);
      eESPstate = eEspSingleConnModeResp;
      Serial.print(F("ESP-REQ-MUX\r\n"));
      break;

    case eEspSingleConnModeResp:
      if (iTime100ms >= 1)
      {
        if (handleRespose(&bRetry , eEspSingleConnModeResp) == true)
        {
          eESPstate = eEspTransparentModeOffReq;
          Serial.print(F("ESP-RESP-MUX\r\n"));
        }
        else if (bRetry == true)
        {
          eESPstate = eEspSingleConnModeReq;
        }
        else
        {
          eESPstate = eEspATReq;
          Serial.print(F("Check connection with ESP\r\n")); //print error if even after set number of retries ESP doesn't respond
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspTransparentModeOffReq:
      hal_uart_tx((char*)("AT+CIPMODE=0\r\n"), 14);
      eESPstate = eEspTransparentModeOffResp;
      Serial.print(F("ESP-REQ-PASS-OFF\r\n"));
      break;

    case eEspTransparentModeOffResp:
      if (iTime100ms >= 1)
      {
        if (handleRespose(&bRetry , eEspTransparentModeOffResp) == true)
        {
          eESPstate = eEspSetupTCPReq;
          Serial.print(F("ESP-RESP-PASS-OFF\r\n"));
        }
        else if (bRetry == true)
        {
          eESPstate = eEspTransparentModeOffReq;
        }
        else
        {
          eESPstate = eEspATReq;
          Serial.print(F("Check connection with ESP\r\n")); //print error if even after set number of retries ESP doesn't respond
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspSetupTCPReq:
      hal_uart_tx((char*)("AT+CIPSTART=\"TCP\",\"io.adafruit.com\",1883\r\n"), 42);
      eESPstate = eEspSetupTCPResp;
      Serial.print(F("ESP-REQ-TCP\r\n"));
      break;

    case eEspSetupTCPResp:
      if (iTime100ms >= 5)
      {
        if (handleRespose(&bRetry , eEspSetupTCPResp) == true)
        {
          eESPstate = eEspTransparentModeOnReq;
          Serial.print(F("ESP-RESP-TCP\r\n"));
        }
        else if (bRetry == true)
        {
          eESPstate = eEspSetupTCPReq;
        }
        else
        {
          eESPstate = eEspATReq;
          Serial.print(F("Check connection with ESP\r\n")); //print error if even after set number of retries ESP doesn't respond
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspTransparentModeOnReq:
      hal_uart_tx((char*)("AT+CIPMODE=1\r\n"), 14);
      eESPstate = eEspTransparentModeOnResp;
      Serial.print(F("ESP-REQ-PASS-ON\r\n"));
      break;

    case eEspTransparentModeOnResp:
      if (iTime100ms >= 1)
      {
        if (handleRespose(&bRetry , eEspTransparentModeOnResp) == true)
        {
          eESPstate = eEspSendReq;
          Serial.print(F("ESP-RESP-PASS-ON\r\n"));
        }
        else if (bRetry == true)
        {
          eESPstate = eEspTransparentModeOnReq;
        }
        else
        {
          eESPstate = eEspexittransparentmodeReq;
          Serial.print(F("Check connection with ESP\r\n")); //print error if even after set number of retries ESP doesn't respond
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspSendReq:
      hal_uart_tx((char*)("AT+CIPSEND\r\n"), 12);
      eESPstate = eEspSendResp;
      Serial.print(F("ESP-REQ-SEND\r\n"));
      break;

    case eEspSendResp:
      if (iTime100ms >= 1)
      {
        if (handleRespose(&bRetry , eEspSendResp) == true)
        {
          eESPstate = eEspSuccessState;
          Serial.print(F("ESP-RESP-SEND\r\n"));
        }
        else if (bRetry == true)
        {
          eESPstate = eEspSendReq;
        }
        else
        {
          eESPstate = eEspexittransparentmodeReq;
          Serial.print(F("Check connection with ESP\r\n")); //print error if even after set number of retries ESP doesn't respond
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspexittransparentmodeReq:
      eESPStatus = eEspInProgress;
      eESPstate = eEspexittransparentmodeResp;
      hal_uart_tx((char*)("+++"), 3);
      Serial.print(F("ESP-REQ-EXT\r\n"));
      break;

    case eEspexittransparentmodeResp:
      if (iTime100ms >= 5)
      {
        if (handleRespose(&bRetry , eEspexittransparentmodeResp) == true)
        {
          eESPstate = eEspATReq;
          Serial.print(F("ESP-RESP-EXT\r\n"));
        }
        else if (bRetry == true)
        {
          eESPstate = eEspexittransparentmodeReq;
        }
        else
        {
          eESPstate = eEspATReq;
        }
        iTime100ms = 0;
      }
      iTime100ms++;
      break;

    case eEspSuccessState:
      ESPInitFlag = true;
      eESPStatus = eEspSuccess;
      break;

    case eEspErrorState:
      eESPStatus = eEspError;
      break;

    default:
      break;
  }
  return eESPStatus;
}

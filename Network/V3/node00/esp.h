enum teESPstate
{
  eEspATReq,
  eEspATResp,
  eEspATE0Req,
  eEspATE0Resp,
  eEspStationModeReq,
  eEspStationModeResp,
  eEspConnectToAccessPointReq,
  eEspConnectToAccessPointResp,
  eEspSingleConnModeReq,
  eEspSingleConnModeResp,
  eEspTransparentModeOffReq,
  eEspTransparentModeOffResp,
  eEspSetupTCPReq,
  eEspSetupTCPResp,
  eEspTransparentModeOnReq,
  eEspTransparentModeOnResp,
  eEspSendReq,
  eEspSendResp,
  eEspexittransparentmode,
  eEspSuccessState,
  eEspErrorState,
} ;

enum teESPstatus
{
  eEspError,
  eEspInProgress,
  eEspSuccess,
};

teESPstatus ESPStateMachine();

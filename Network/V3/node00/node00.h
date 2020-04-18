
/*Global Declarationns*/

/*Macro Declarations*/
#define RxMaxBuffSize                     (uint8_t)200
#define RxDataMaxBuffSize                 (uint8_t)5

#define MQTT_CTRL_CONNECT                 0x10
#define MQTT_CTRL_CONNECTACK              0x20
#define MQTT_CTRL_PUBLISH                 0x30
#define MQTT_CTRL_PUBACK                  0x40
#define MQTT_CTRL_PUBREC                  0x50
#define MQTT_CTRL_PUBREL                  0x60
#define MQTT_CTRL_PUBCOMP                 0x70
#define MQTT_CTRL_SUBSCRIBE               0x80
#define MQTT_CTRL_SUBACK                  0x90
#define MQTT_CTRL_UNSUBSCRIBE             0xA0
#define MQTT_CTRL_UNSUBACK                0xB0
#define MQTT_CTRL_PINGREQ                 0xC0
#define MQTT_CTRL_PINGRESP                0xD0
#define MQTT_CTRL_DISCONNECT              0xE0

#define MQTT_CTRL_CONNECT_FLAG            0x0
#define MQTT_CTRL_CONNECTACK_FLAG         0x0
#define MQTT_CTRL_PUBLISH_FLAG            0x0      //DUP,QoS,Qos,RETAIN
#define MQTT_CTRL_PUBACK_FLAG             0x0
#define MQTT_CTRL_PUBREC_FLAG             0x0
#define MQTT_CTRL_PUBREL_FLAG             0x2
#define MQTT_CTRL_PUBCOMP_FLAG            0x0
#define MQTT_CTRL_SUBSCRIBE_FLAG          0x2
#define MQTT_CTRL_SUBACK_FLAG             0x0
#define MQTT_CTRL_UNSUBSCRIBE_FLAG        0x2
#define MQTT_CTRL_UNSUBACK_FLAG           0x0
#define MQTT_CTRL_PINGREQ_FLAG            0x0
#define MQTT_CTRL_PINGRESP_FLAG           0x0
#define MQTT_CTRL_DISCONNECT_FLAG         0x0

#define MQTT_CONN_KEEPALIVE               (uint16_t)60 //no. of seconds for which connection is to be kept alive 

#define MQTT_QOS_2                        0x2 //Quality of service level 2
#define MQTT_QOS_1                        0x1 //Quality of service level 1
#define MQTT_QOS_0                        0x0 //Quality of service level 0
#define MQTT_RETAIN                       0x1 //Packet to be retained by server
#define MQTT_NO_RETAIN                    0x0 //Packet to be discarded after processing

#define MQTT_CONN_USERNAMEFLAG            0x80 //If username is present
#define MQTT_CONN_PASSWORDFLAG            0x40 //If password is present (applicable only is username flag is high)
#define MQTT_CONN_WILLRETAIN              0x20
#define MQTT_CONN_WILLQOS_1               0x08
#define MQTT_CONN_WILLQOS_2               0x18
#define MQTT_CONN_WILLFLAG                0x04
#define MQTT_CONN_CLEANSESSION            0x02 //Start with a clean session

#define MQTT_PROTOCOL_LEVEL               0x04 //Protocol level 4 as per MQTT 3.1.1

#define MQTT_USERNAME                     "AniIOT"
#define MQTT_PASSWORD                     "1d2f2a184b57420e9f95b216d614a181" //AIO key
#define MQTT_TOPIC                        "AniIOT/feeds/LightD"

/*Variable Declarations*/
typedef enum 
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
  eEspexittransparentmodeReq,
  eEspexittransparentmodeResp,
  eEspSuccessState,
  eEspErrorState,
}teESPstate;

typedef enum
{
  eEspError,
  eEspInProgress,
  eEspSuccess,
}teESPstatus;

typedef enum
{
  eMQTTInit,
  eMQTTConnectReq,
  eMQTTConnectACK,
  eMQTTSubscribeReq,
  eMQTTSubscribeACK,
  eMQTTPublishReq,
  eMQTTPublishACK,
  eMQTTSuccessState,
  eMQTTErrorState,
}teMQTTState;

typedef enum 
{
  eMQTTError,
  eMQTTInProgress,
  eMQTTSuccess,
}teMQTTstatus;

static unsigned char RxBuffer[RxMaxBuffSize] = {0};
static unsigned char DataBuffer[RxDataMaxBuffSize] = {0};
boolean ESPInitFlag = false;
boolean MQTTInitFlag = false;

/*Function Declarations*/
void hal_uart_Init();
boolean hal_uart_tx(char* pTxBuff, uint8_t utxCount);
teMQTTstatus MQTTStateMachine();
teESPstatus ESPStateMachine();
void connectToBroker();
void subscribeToTopic(char * ptrTopic);
void publishToTopic(char * ptrTopic, char* ptrData);
void pingToServer();

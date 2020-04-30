#include <NeoSWSerial.h>

/*Global Declarations*/

/*Macro Declarations*/
#define testenable 1

/*switch pins*/
#define GPIOD_S0                 ((uint8_t)0x40)  //mux select line0 D6
#define GPIOD_S1                 ((uint8_t)0x80)  //mux select line1 D7
#define GPIOB_S2                 ((uint8_t)0x01)  //mux select line2 D8
#define GPIOD_OP                 ((uint8_t)0x01)  //mux output line  D0
#if testenable
#define muxOPpin (PIND & 0x08) >> 3
#else
#define muxOPpin (PIND & 0x01)
#endif

/*Software Serial Baudrate*/
#define ESP_UART_BAUD                     9600
/*Hardware Serial Baudrate*/
#define DEBUG_UART_BAUD                   9600
/*Software UART pins - ESP8266 pins*/
#define ESP_RX                            ((uint8_t)0x04)  //esp uart        D4
#define ESP_TX                            ((uint8_t)0x05)  //esp uart        D5
/*relay pins*/
#define GPIOD_R1                          ((uint8_t)0x04)  //output relay 1  D2
#define GPIOD_R2                          ((uint8_t)0x08)  //output relay 2  D3
#define GPIOC_R3                          ((uint8_t)0x20)  //output relay 3  D19 no AF
#define GPIOC_R4                          ((uint8_t)0x10)  //output relay 4  D18 no AF
#define GPIOC_R5                          ((uint8_t)0x08)  //output relay 5  D17 no AF
#define GPIOC_R6                          ((uint8_t)0x04)  //output relay 6  D16 no AF
#define GPIOC_R7                          ((uint8_t)0x02)  //output relay 7  D15 no AF
#define GPIOC_R8                          ((uint8_t)0x01)  //output relay 8  D14 no AF
/*SPI pins - NRF24L01 pins*/
#define GPIOB_CE                          ((uint8_t)0x02)  //nrf comm ce     D9
#define GPIOB_CSN                         ((uint8_t)0x04)  //nrf comm csn    D10
#define GPIOB_MOSI                        ((uint8_t)0x08)  //nrf spi mosi    D11
#define GPIOB_MISO                        ((uint8_t)0x10)  //nrf spi miso    D12
#define GPIOB_SCK                         ((uint8_t)0x20)  //nrf spi sck     D13

#define RxMaxBuffSize                     (uint8_t)200
#define RxDataMaxBuffSize                 (uint8_t)10

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

#define MQTT_POS_ACK                      0x0

#define MQTT_CONN_KEEPALIVE               (uint16_t)240 //no. of seconds for which connection is to be kept alive 

#define MQTT_QOS_2                        (uint8_t)0x2 //Quality of service level 2
#define MQTT_QOS_1                        (uint8_t)0x1 //Quality of service level 1
#define MQTT_QOS_0                        (uint8_t)0x0 //Quality of service level 0
#define MQTT_RETAIN                       0x1 //Packet to be retained by server
#define MQTT_NO_RETAIN                    0x0 //Packet to be discarded after processing

#define MQTT_CONN_USERNAMEFLAG            0x80 //If username is present
#define MQTT_CONN_PASSWORDFLAG            0x40 //If password is present (applicable only is username flag is high)
#define MQTT_CONN_WILLRETAIN              0x20 //Sates if the will message is to be retained by server after disconnect
#define MQTT_CONN_WILLQOS_1               0x08 //Will message QoS
#define MQTT_CONN_WILLQOS_2               0x18 //Will message QoS
#define MQTT_CONN_WILLFLAG                0x04 //States if a will message is present
#define MQTT_CONN_CLEANSESSION            0x02 //Start with a clean session

#define MQTT_PROTOCOL_LEVEL               0x04 //Protocol level 4 as per MQTT 3.1.1

#define MQTT_USERNAME                     "AniIOT"
#define MQTT_PASSWORD                     "1d2f2a184b57420e9f95b216d614a181" //AIO key

#define MQTT_TOPIC1                        "AniIOT/feeds/MOOD"
#define MQTT_TOPIC2                        "AniIOT/feeds/main.f1"
#define MQTT_TOPIC3                        "AniIOT/feeds/main.l1"
#define MQTT_TOPIC4                        "AniIOT/feeds/main.l2"
#define MQTT_TOPIC5                        "AniIOT/feeds/main.l3"
#define MQTT_TOPIC6                        "AniIOT/feeds/main.l4"
#define MQTT_TOPIC7                        "AniIOT/feeds/main.l5"
#define MQTT_TOPIC8                        "AniIOT/feeds/main.l6"
#define MQTT_TOPIC9                        "AniIOT/feeds/main.ac"
#define MQTT_TOPIC10                       "AniIOT/feeds/pass.l1"
#define MQTT_TOPIC11                       "AniIOT/feeds/pass.l2"
#define MQTT_TOPIC12                       "AniIOT/feeds/toil.f1"
#define MQTT_TOPIC13                       "AniIOT/feeds/toil.l1"
#define MQTT_TOPIC14                       "AniIOT/feeds/toil.l2"
#define MQTT_TOPIC15                       "AniIOT/feeds/toil.l3"



typedef enum
{
  Fan,
  Yellow1,
  Yellow2,
  White1,
  White2,
  White3,
  White4,
  Passage,
  Loft,
  Exaust,
  ToiletWhite,
  ToiletYellow,
  WaterHeater,
  MaxSwitches,
} teSwitchCategory;

/*Variable Declarations*/
typedef enum
{
  eEspATReq,
  eEspATResp,
  eEspATE0Req,
  eEspATE0Resp,
  eEspStationModeReq,
  eEspStationModeResp,
  eEspCheckConnectionToAccessPointReq,
  eEspCheckConnectionToAccessPointResp,
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
} teESPstate;

typedef enum
{
  eEspError,
  eEspInProgress,
  eEspSuccess,
} teESPstatus;

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
} teMQTTState;

typedef enum
{
  eMQTTError,
  eMQTTInProgress,
  eMQTTSuccess,
} teMQTTstatus;

unsigned long prevMillis100mstask = 0;        //MQTT State Machine
unsigned long prevMillis10000mstask = 0;      //Ping to server
unsigned long prevMillis50mstask = 0;         //Read Switches
unsigned long currMillis  = 0;                //Current millis value
unsigned char RxBuffer[RxMaxBuffSize] = {0};
boolean MQTTSwitchBuffer[MaxSwitches] = {0};
uint16_t SubPacketID = 0x0001;
uint16_t PubPacketID = 0x0001;
uint8_t rxBufferCount = 0;
uint8_t u8pingCount = 0;
uint8_t subscribeCount = 0;
boolean ESPInitFlag = false;
boolean MQTTInitFlag = false;
boolean MoodChill = false;
boolean MoodWork = false;
boolean MoodAOff = false;

/*Function Declarations*/
void hal_uart_Init();
boolean hal_uart_tx(char* pTxBuff, uint8_t utxCount);

void TimedFunctionCaller_Init();
void TimedFunctionCaller();

teESPstatus ESPStateMachine();
void resetESP();

teMQTTstatus MQTTStateMachine();
void publishIfMoodReq();
void resetMQTT();
void connectToBroker(uint8_t u8connectFlags, unsigned char* ucClientID, unsigned char* ucUsername, unsigned char* ucPassword, uint16_t u16KeepAliveTimeS);
void subscribeToTopic(char * ptrTopic, char * ptrTopic2, uint8_t uiQoS);
void publishToTopic(char * ptrTopic, char* ptrData, uint8_t uiQoS, uint8_t u8RetainFlag);
void pingToServer();

void hal_gpio_init();
void ReadSwitches();

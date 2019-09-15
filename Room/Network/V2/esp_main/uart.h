#define uartBaud 38400
#define txmaxBytes 4
#define rxmaxBytes 2
#define slave_add 130
#define reqFuncCode 24

/*Global-Variables*/
unsigned char RxBuffer[rxmaxBytes] = {0};
uint8_t rxCount = 0;

void uart_init();
//boolean rx_uart(unsigned char* pcRxBuffer);
void tx_uart(unsigned char *packet, uint16_t packetlength);
boolean modbusCheckReq(unsigned char* rxBuff);
void modbusSendData(uint16_t* pu16Data);

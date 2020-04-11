#define ESPRxMaxBuffSize 50

static char ESPRxBuffer[ESPRxMaxBuffSize] = {0};

void hal_uart_Init();
boolean hal_uart_tx(char* pTxBuff, uint8_t utxCount);

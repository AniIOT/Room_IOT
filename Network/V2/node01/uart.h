void uart_init();
bool modbusReadData();
bool checkData();
uint16_t getData();
static void uart_isr(unsigned char c);

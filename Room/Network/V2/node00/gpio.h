/*MACROS*/
/*switch pins*/
#define GPIOD_S0                 ((uint8_t)0x40)  //mux select line0 D6
#define GPIOD_S1                 ((uint8_t)0x80)  //mux select line1 D7
#define GPIOB_S2                 ((uint8_t)0x01)  //mux select line2 D8
#define GPIOD_OP                 ((uint8_t)0x01)  //mux output line  D0

#if testenable
#define muxOPpin (PIND & 0x04) >> 2
#else
#define muxOPpin (PIND & 0x01)
#endif

/*relay pins*/
#define GPIOD_R1                 ((uint8_t)0x04)  //output relay 1  D2
#define GPIOD_R2                 ((uint8_t)0x08)  //output relay 2  D3
#define GPIOC_R3                 ((uint8_t)0x20)  //output relay 3  D19 no AF
#define GPIOC_R4                 ((uint8_t)0x10)  //output relay 4  D18 no AF
#define GPIOC_R5                 ((uint8_t)0x08)  //output relay 5  D17 no AF
#define GPIOC_R6                 ((uint8_t)0x04)  //output relay 6  D16 no AF
#define GPIOC_R7                 ((uint8_t)0x02)  //output relay 7  D15 no AF
#define GPIOC_R8                 ((uint8_t)0x01)  //output relay 8  D14 no AF

/*SPI pins - NRF24L01 pins*/
#define GPIOB_CE                 ((uint8_t)0x02)  //nrf comm ce     D9
#define GPIOB_CSN                ((uint8_t)0x04)  //nrf comm csn    D10
#define GPIOB_MOSI               ((uint8_t)0x08)  //nrf spi mosi    D11
#define GPIOB_MISO               ((uint8_t)0x10)  //nrf spi miso    D12
#define GPIOB_SCK                ((uint8_t)0x20)  //nrf spi sck     D13

/*Software UART pins - ESP8266 pins*/
#define GPIOD_RX                 ((uint8_t)0x10)  //esp uart        D4
#define GPIOD_TX                 ((uint8_t)0x20)  //esp uart        D5

/*Local Variables*/


/*Global Variables*/
extern boolean SwitchRead_Buffer[];

/*function declarations*/
void gpio_init();
void read_switches();
void write_Relays();

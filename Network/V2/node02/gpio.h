/*MACROS*/
/*switch pins*/
#define GPIOB_S1                 ((PINB & 0x01) >> 0)  //switch1 input  D8
#define GPIOD_S2                 ((PIND & 0x80) >> 7)  //switch2 input  D7
#define GPIOD_S3                 ((PIND & 0x40) >> 6)  //switch3 input  D6
#define GPIOD_S4                 ((PIND & 0x20) >> 5)  //switch4 input  D5
#define GPIOD_S5                 ((PIND & 0x10) >> 4)  //switch5 input  D4
#define GPIOD_S6                 ((PIND & 0x08) >> 3)  //switch6 input  D3

/*relay pins*/
#define GPIOC_R1                 ((uint8_t)0x20)  //output relay 1  D19
#define GPIOC_R2                 ((uint8_t)0x10)  //output relay 2  D18
#define GPIOC_R3                 ((uint8_t)0x08)  //output relay 3  D17
#define GPIOC_R6                 ((uint8_t)0x04)  //output relay 6  D16
#define GPIOC_R5                 ((uint8_t)0x02)  //output relay 5  D15
#define GPIOC_R4                 ((uint8_t)0x01)  //output relay 4  D14

/*SPI pins - NRF24L01 pins*/
#define GPIOB_CE                 ((uint8_t)0x02)  //nrf comm ce     D9
#define GPIOB_CSN                ((uint8_t)0x04)  //nrf comm csn    D10
#define GPIOB_MOSI               ((uint8_t)0x08)  //nrf spi mosi    D11
#define GPIOB_MISO               ((uint8_t)0x10)  //nrf spi miso    D12
#define GPIOB_SCK                ((uint8_t)0x20)  //nrf spi sck     D13

/*Local Variables*/


/*Global Variables*/
extern boolean SwitchRead_Buffer[];

/*function declarations*/
void gpio_init();
void read_switches();
void write_Relays();

#include "gpio.h"

/*Global variables*/
boolean switchRead_Buffer[8] = {0};
/*Local Variables*/
enum StructSwitches
{
  switch1,
  switch2,
  switch3,
  switch4,
  switch5,
  switch6,
  switch7,
  switch8,
};

StructSwitches ReadSwitches = switch1;

void gpio_init()
{
  /*Output config*/
  DDRB |= GPIOB_S2;
  DDRC |= GPIOC_R3 | GPIOC_R4 | GPIOC_R5 | GPIOC_R6 | GPIOC_R7 | GPIOC_R8;
#if testenable
  DDRD |= GPIOD_R2 | GPIOD_S0 | GPIOD_S1 | GPIOD_TX;
#else
  DDRD |= GPIOD_R1 | GPIOD_R2 | GPIOD_S0 | GPIOD_S1 | GPIOD_TX;
#endif
  /*Input config*/
#if testenable
  DDRD &= ~GPIOD_R1;
#endif
  DDRD &= ~GPIOD_OP;

  /*SPI pin config*/

}

void read_switches()
{
  switch (ReadSwitches)
  {
    case switch1: //7
      PORTB |=  GPIOB_S2;
      delay(1);
      switchRead_Buffer[0] = muxOPpin;
      ReadSwitches = switch2;
      break;

    case switch2: //6
      PORTD &= ~GPIOD_S0;
      delay(1);
      switchRead_Buffer[1] = muxOPpin;
      ReadSwitches = switch3;
      break;

    case switch3: //0
      PORTD &= ~GPIOD_S1;
      PORTB &= ~GPIOB_S2;
      delay(1);
      switchRead_Buffer[2] = muxOPpin;
      ReadSwitches = switch4;
      break;

    case switch4: //1
      PORTD |=  GPIOD_S0;
      delay(1);
      switchRead_Buffer[3] = muxOPpin;
      ReadSwitches = switch5;
      break;

    case switch5: //2
      PORTD &= ~GPIOD_S0;
      PORTD |=  GPIOD_S1;
      delay(1);
      switchRead_Buffer[4] = muxOPpin;
      ReadSwitches = switch6;
      break;

    case switch6: //5
      PORTD |=  GPIOD_S0;
      PORTD &= ~GPIOD_S1;
      PORTB |=  GPIOB_S2;
      delay(1);
      switchRead_Buffer[5] = muxOPpin;
      ReadSwitches = switch7;
      break;

    case switch7:  //4
      PORTD &= ~GPIOD_S0;
      delay(1);
      switchRead_Buffer[6] = muxOPpin;
      ReadSwitches = switch8;
      break;

    case switch8: //3
      PORTD |=  GPIOD_S0;
      PORTD |=  GPIOD_S1;
      PORTB &= ~GPIOB_S2;
      delay(1);
      switchRead_Buffer[7] = muxOPpin;
      ReadSwitches = switch1;
      break;
  }
}


void write_Relays()
{
    if (switchRead_Buffer[5]) //fan
      PORTD |=  GPIOD_R1;
    else
      PORTD &= ~GPIOD_R1;
      
    if (switchRead_Buffer[0]) //yi
      PORTD |=  GPIOD_R2;
    else
      PORTD &= ~GPIOD_R2;
      
    if (switchRead_Buffer[1]) //2 w1
      PORTC |=  GPIOC_R3;
    else
      PORTC &= ~GPIOC_R3;
      
    if (switchRead_Buffer[2]) //w2
      PORTC |=  GPIOC_R4;
    else
      PORTC &= ~GPIOC_R4;
      
    if (switchRead_Buffer[3]) //w3
      PORTC |=  GPIOC_R5;
    else
      PORTC &= ~GPIOC_R5;
      
    if (switchRead_Buffer[7]) //w4
      PORTC |=  GPIOC_R6;
    else
      PORTC &= ~GPIOC_R6;
      
    if (switchRead_Buffer[4]) //w4
      PORTC |=  GPIOC_R7;
    else
      PORTC &= ~GPIOC_R7;
      
    if (switchRead_Buffer[6]) //yo
      PORTC |=  GPIOC_R8;
    else
      PORTC &= ~GPIOC_R8;
}

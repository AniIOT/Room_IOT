#include "gpio.h"

/*Global variables*/
boolean SwitchRead_Buffer[8] = {0};
boolean flag = 0;

/*Local Variables*/
boolean currSwitchRead_Buffer[8] = {0};
boolean prevSwitchRead_Buffer[8] = {0};
uint8_t i = 0;
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
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = muxOPpin;
      ReadSwitches = switch2;
      i++;
      break;

    case switch2: //6
      PORTD &= ~GPIOD_S0;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = muxOPpin;
      i++;
      ReadSwitches = switch3;
      break;

    case switch3: //0
      PORTD &= ~GPIOD_S1;
      PORTB &= ~GPIOB_S2;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = muxOPpin;
      i++;
      ReadSwitches = switch4;
      break;

    case switch4: //1
      PORTD |=  GPIOD_S0;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = muxOPpin;
      i++;
      ReadSwitches = switch5;
      break;

    case switch5: //2
      PORTD &= ~GPIOD_S0;
      PORTD |=  GPIOD_S1;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = muxOPpin;
      i++;
      ReadSwitches = switch6;
      break;

    case switch6: //5
      PORTD |=  GPIOD_S0;
      PORTD &= ~GPIOD_S1;
      PORTB |=  GPIOB_S2;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = muxOPpin;
      i++;
      ReadSwitches = switch7;
      break;

    case switch7:  //4
      PORTD &= ~GPIOD_S0;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = muxOPpin;
      i++;
      ReadSwitches = switch8;
      break;

    case switch8: //3
      PORTD |=  GPIOD_S0;
      PORTD |=  GPIOD_S1;
      PORTB &= ~GPIOB_S2;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = muxOPpin;
      i = 0;
      flag = not flag;
      ReadSwitches = switch1;
      break;
  }
}


void write_Relays()
{
  if (SwitchRead_Buffer[5]) //fan
    PORTD |=  GPIOD_R1;
  else
    PORTD &= ~GPIOD_R1;

  if (SwitchRead_Buffer[0]) //yi
    PORTD |=  GPIOD_R2;
  else
    PORTD &= ~GPIOD_R2;

  if (SwitchRead_Buffer[1]) //2 w1
    PORTC |=  GPIOC_R3;
  else
    PORTC &= ~GPIOC_R3;

  if (SwitchRead_Buffer[2]) //w2
    PORTC |=  GPIOC_R4;
  else
    PORTC &= ~GPIOC_R4;

  if (SwitchRead_Buffer[3]) //w3
    PORTC |=  GPIOC_R5;
  else
    PORTC &= ~GPIOC_R5;

  if (SwitchRead_Buffer[7]) //w4
    PORTC |=  GPIOC_R6;
  else
    PORTC &= ~GPIOC_R6;

  if (SwitchRead_Buffer[4]) //w4
    PORTC |=  GPIOC_R7;
  else
    PORTC &= ~GPIOC_R7;

  if (SwitchRead_Buffer[6]) //yo
    PORTC |=  GPIOC_R8;
  else
    PORTC &= ~GPIOC_R8;
}

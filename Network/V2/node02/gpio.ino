#include "gpio.h"

/*Global variables*/
static boolean SwitchRead_Buffer[5] = {1};
boolean flag = 0;

/*Local Variables*/
boolean currSwitchRead_Buffer[5] = {1};
boolean prevSwitchRead_Buffer[5] = {1};
uint8_t i = 0;
enum StructSwitches
{
  switch1,
  switch2,
  switch3,
  switch4,
  switch5,
};
StructSwitches ReadSwitches = switch1;

void gpio_init()
{
  /*Output config*/
  DDRC |= GPIOC_R1 | GPIOC_R2 | GPIOC_R3 | GPIOC_R4 | GPIOC_R5;
  /*Input config*/
  DDRB &= ~GPIOB_S1;
  DDRD &= ~(GPIOD_S2 | GPIOD_S3 | GPIOD_S4 | GPIOD_S5);

  /*SPI pin config*/

}

void read_switches()
{
  switch (ReadSwitches)
  {
    case switch1:
      if (flag)
        currSwitchRead_Buffer[i] = GPIOB_S1;
      else
        prevSwitchRead_Buffer[i] = GPIOB_S1;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = prevSwitchRead_Buffer[i];
      i++;

    case switch2:
      if (flag)
        currSwitchRead_Buffer[i] = GPIOD_S2;
      else
        prevSwitchRead_Buffer[i] = GPIOD_S2;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = prevSwitchRead_Buffer[i];
      i++;

    case switch3:
      if (flag)
        currSwitchRead_Buffer[i] = GPIOD_S3;
      else
        prevSwitchRead_Buffer[i] = GPIOD_S3;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = prevSwitchRead_Buffer[i];
      i++;

    case switch4:
      if (flag)
        currSwitchRead_Buffer[i] = GPIOD_S4;
      else
        prevSwitchRead_Buffer[i] = GPIOD_S4;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = prevSwitchRead_Buffer[i];
      i++;

    case switch5:
      if (flag)
        currSwitchRead_Buffer[i] = GPIOD_S5;
      else
        prevSwitchRead_Buffer[i] = GPIOD_S5;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        SwitchRead_Buffer[i] = prevSwitchRead_Buffer[i];
      i = 0;
      flag = not flag;
      break;
  }
}


void write_Relays()
{
  if (!SwitchRead_Buffer[0]) //basin
    PORTC |=  GPIOC_R1;
  else
    PORTC &= ~GPIOC_R1;

  if (!SwitchRead_Buffer[1]) //passage
    PORTC |=  GPIOC_R2;
  else
    PORTC &= ~GPIOC_R2;

  if (!SwitchRead_Buffer[2]) //loft
    PORTC |=  GPIOC_R3;
  else
    PORTC &= ~GPIOC_R3;

  if (!SwitchRead_Buffer[3]) //toilet
    PORTC |=  GPIOC_R4;
  else
    PORTC &= ~GPIOC_R4;

  if (!SwitchRead_Buffer[4]) //exhaust
    PORTC |=  GPIOC_R5;
  else
    PORTC &= ~GPIOC_R5;
}

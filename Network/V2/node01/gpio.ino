#include "gpio.h"

/*Global variables*/
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
        switchBuffer[i] = prevSwitchRead_Buffer[i];
      i++;

    case switch2: //6
      PORTD &= ~GPIOD_S0;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        switchBuffer[i] = prevSwitchRead_Buffer[i];
      i++;

    case switch3: //0
      PORTD &= ~GPIOD_S1;
      PORTB &= ~GPIOB_S2;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        switchBuffer[i] = prevSwitchRead_Buffer[i];
      i++;

    case switch4: //1
      PORTD |=  GPIOD_S0;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        switchBuffer[i] = prevSwitchRead_Buffer[i];
      i++;

    case switch5: //2
      PORTD &= ~GPIOD_S0;
      PORTD |=  GPIOD_S1;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        switchBuffer[i] = prevSwitchRead_Buffer[i];
      i++;

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
        switchBuffer[i] = prevSwitchRead_Buffer[i];
      i++;

    case switch7:  //4
      PORTD &= ~GPIOD_S0;
      delay(1);
      if (flag)
        currSwitchRead_Buffer[i] = muxOPpin;
      else
        prevSwitchRead_Buffer[i] = muxOPpin;
      if ((flag == 1) && (prevSwitchRead_Buffer[i] == currSwitchRead_Buffer[i]))
        switchBuffer[i] = prevSwitchRead_Buffer[i];
      i++;

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
        switchBuffer[i] = prevSwitchRead_Buffer[i];
      i = 0;
      flag = not flag;
      break;
  }
//  printsBuffer();
}

void printsBuffer()
{
  Serial.print(switchBuffer[0]);
  Serial.print(switchBuffer[1]);
  Serial.print(switchBuffer[2]);
  Serial.print(switchBuffer[3]);
  Serial.print(switchBuffer[4]);
  Serial.print(switchBuffer[5]);
  Serial.print(switchBuffer[6]);
  Serial.print(switchBuffer[7]);
}

void write_Relays()
{
  if (switchBuffer[0] xor wifiBuffer[0]) //yi
  {
    PORTD |=  GPIOD_R2;
  }
  else
    PORTD &= ~GPIOD_R2;

  if (switchBuffer[1] xor wifiBuffer[1]) //w1
    PORTC |=  GPIOC_R3;
  else
    PORTC &= ~GPIOC_R3;

  if (switchBuffer[2] xor wifiBuffer[2]) //w2
    PORTC |=  GPIOC_R4;
  else
    PORTC &= ~GPIOC_R4;

  if (switchBuffer[3] xor wifiBuffer[3]) //w3
    PORTC |=  GPIOC_R5;
  else
    PORTC &= ~GPIOC_R5;

  if (switchBuffer[4] xor wifiBuffer[4]) //w4
    PORTC |=  GPIOC_R6;
  else
    PORTC &= ~GPIOC_R6;

  if (switchBuffer[5] xor wifiBuffer[5]) //fan
    PORTD |=  GPIOD_R1;
  else
    PORTD &= ~GPIOD_R1;

  if (switchBuffer[6] xor wifiBuffer[6]) //yo
    PORTC |=  GPIOC_R8;
  else
    PORTC &= ~GPIOC_R8;

  if (switchBuffer[7] xor wifiBuffer[7]) //e1
    PORTC |=  GPIOC_R7;
  else
    PORTC &= ~GPIOC_R7;
}

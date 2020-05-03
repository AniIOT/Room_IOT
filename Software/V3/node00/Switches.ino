void hal_gpio_init()
{
  /*Output config*/
  //  DDRB |= GPIOB_S2;
  //  DDRC |= GPIOC_R3 | GPIOC_R4 | GPIOC_R5 | GPIOC_R6 | GPIOC_R7 | GPIOC_R8;
  //#if testenable
  //  DDRD |= GPIOD_R2 | GPIOD_S0 | GPIOD_S1;
  //#else
  //  DDRD |= GPIOD_R1 | GPIOD_R2 | GPIOD_S0 | GPIOD_S1;
  //#endif
  //
  //  /*Input config*/
  //#if testenable
  //  DDRD &= ~GPIOD_R1;
  //#endif
  //  DDRD &= ~GPIOD_OP;
  //
  //  /*Digital write zero*/
  //  PORTD &= ~GPIOD_S0;
  //  PORTD &= ~GPIOD_S1;
  //  PORTB &= ~GPIOB_S2;

  pinMode(GPIOD_DS0, OUTPUT);
  pinMode(GPIOD_DS1, OUTPUT);
  pinMode(GPIOB_DS2, OUTPUT);

  digitalWrite(GPIOD_DS0, LOW);
  digitalWrite(GPIOD_DS1, LOW);
  digitalWrite(GPIOB_DS2, LOW);
  /*SPI pin config*/

}

void ReadSwitches()
{
  static boolean bPrevSwitchState[MaxSwitches] = {false, false, false, false, false, false, false, false, false, false, false, false, false};
  static boolean bSwitchChangeState[MaxSwitches] = {true, true, true, true, true, true, true, true, true, true, true, true, true};
  boolean bCurrSwitchState = false;

  /*=============Yellow Internal==============*/
  //  PORTD |=  GPIOD_S0;
  //  PORTD |=  GPIOD_S1;
  //  PORTB |=  GPIOB_S2;
  //  digitalWrite(GPIOD_DS0, HIGH);
//  digitalWrite(GPIOD_DS1, HIGH);
//  digitalWrite(GPIOB_DS2, HIGH);
  delay(1);
  bCurrSwitchState = muxOPpin;
  if (bPrevSwitchState[Yellow2] == bCurrSwitchState)
  {
    if (bSwitchChangeState[Yellow2] == true)
    {
      //write value to buffer here
      Serial.println(bCurrSwitchState);
      bSwitchChangeState[Yellow2] = false;
    }
  }
  else
  {
    bPrevSwitchState[Yellow2] = bCurrSwitchState;
    bSwitchChangeState[Yellow2] = true;
  }

  //  PORTD &= ~GPIOD_S0;
  //  PORTD |=  GPIOD_S1;
  //  PORTB |=  GPIOB_S2;
//  digitalWrite(GPIOD_DS0, LOW);
  delay(1);
  bCurrSwitchState = muxOPpin;
  if (bPrevSwitchState[White1] == bCurrSwitchState)
  {
    if (bSwitchChangeState[White1] == true)
    {
      //write value to buffer here
      Serial.println(bCurrSwitchState);
      bSwitchChangeState[White1] = false;
    }
  }
  else
  {
    bPrevSwitchState[White1] = bCurrSwitchState;
    bSwitchChangeState[White1] = true;
  }

  //  delay(1);
  //  bCurrSwitchState = muxOPpin;
  //  if (bPrevSwitchState[Yellow2] == bCurrSwitchState)
  //  {
  //    if (bSwitchChangeState[Yellow2] == true)
  //    {
  //      //write value to buffer here
  //      Serial.println(bCurrSwitchState);
  //      bSwitchChangeState[Yellow2] = false;
  //    }
  //  }
  //  else
  //  {
  //    bPrevSwitchState[Yellow2] = bCurrSwitchState;
  //    bSwitchChangeState[Yellow2] = true;
  //  }
  //
  //  delay(1);
  //  bCurrSwitchState = muxOPpin;
  //  if (bPrevSwitchState[Yellow2] == bCurrSwitchState)
  //  {
  //    if (bSwitchChangeState[Yellow2] == true)
  //    {
  //      //write value to buffer here
  //      Serial.println(bCurrSwitchState);
  //      bSwitchChangeState[Yellow2] = false;
  //    }
  //  }
  //  else
  //  {
  //    bPrevSwitchState[Yellow2] = bCurrSwitchState;
  //    bSwitchChangeState[Yellow2] = true;
  //  }
  //
  //  delay(1);
  //  bCurrSwitchState = muxOPpin;
  //  if (bPrevSwitchState[Yellow2] == bCurrSwitchState)
  //  {
  //    if (bSwitchChangeState[Yellow2] == true)
  //    {
  //      //write value to buffer here
  //      Serial.println(bCurrSwitchState);
  //      bSwitchChangeState[Yellow2] = false;
  //    }
  //  }
  //  else
  //  {
  //    bPrevSwitchState[Yellow2] = bCurrSwitchState;
  //    bSwitchChangeState[Yellow2] = true;
  //  }
  //
  //  delay(1);
  //  bCurrSwitchState = muxOPpin;
  //  if (bPrevSwitchState[Yellow2] == bCurrSwitchState)
  //  {
  //    if (bSwitchChangeState[Yellow2] == true)
  //    {
  //      //write value to buffer here
  //      Serial.println(bCurrSwitchState);
  //      bSwitchChangeState[Yellow2] = false;
  //    }
  //  }
  //  else
  //  {
  //    bPrevSwitchState[Yellow2] = bCurrSwitchState;
  //    bSwitchChangeState[Yellow2] = true;
  //  }
  //
  //  delay(1);
  //  bCurrSwitchState = muxOPpin;
  //  if (bPrevSwitchState[Yellow2] == bCurrSwitchState)
  //  {
  //    if (bSwitchChangeState[Yellow2] == true)
  //    {
  //      //write value to buffer here
  //      Serial.println(bCurrSwitchState);
  //      bSwitchChangeState[Yellow2] = false;
  //    }
  //  }
  //  else
  //  {
  //    bPrevSwitchState[Yellow2] = bCurrSwitchState;
  //    bSwitchChangeState[Yellow2] = true;
  //  }
  //
  //  delay(1);
  //  bCurrSwitchState = muxOPpin;
  //  if (bPrevSwitchState[Yellow2] == bCurrSwitchState)
  //  {
  //    if (bSwitchChangeState[Yellow2] == true)
  //    {
  //      //write value to buffer here
  //      Serial.println(bCurrSwitchState);
  //      bSwitchChangeState[Yellow2] = false;
  //    }
  //  }
  //  else
  //  {
  //    bPrevSwitchState[Yellow2] = bCurrSwitchState;
  //    bSwitchChangeState[Yellow2] = true;
  //  }
}

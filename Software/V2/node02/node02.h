/*Includes*/
#include "rf_comm.h"
#include "gpio.h"
#include "wdg.h"

#define testenable 0 

/*Global variables*/
extern boolean SwitchRead_Buffer[];

/*local variables*/
enum StructState
{
  readSwitches,
  readRfData,
  writetoRelays,
};
//state initialization
StructState machine_state = readSwitches;

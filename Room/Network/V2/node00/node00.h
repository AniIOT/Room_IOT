/*Includes*/
#include "rf_comm.h"
#include "wifi_comm.h"
#include "gpio.h"
#include "uart.h"
#include "wdg.h"

#define testenable 0 

/*Global variables*/
extern uint8_t panelOne_data;
extern uint8_t panelTwo_data;
extern boolean switchRead_Buffer[];

/*local variables*/
enum StructState
{
  readSwitches,
  readWifiData,
  writetoRelays,
};
//state initialization
StructState machine_state = readWifiData;

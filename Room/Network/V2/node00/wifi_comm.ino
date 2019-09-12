#include "wifi_comm.h"
#include "uart.h"

uint8_t panelOne_data = 0;
uint8_t panelTwo_data = 0;

void wifi_comm()
{
  modbusReqData();
  modbusReadData();

  panelTwo_data = u16Data;
  panelOne_data = u16Data>>8;
}

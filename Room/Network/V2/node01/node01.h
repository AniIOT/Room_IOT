#define testenable 0

/*Global variables*/
extern boolean SwitchRead_Buffer[];

/*local variables*/
enum StructState
{
  readSwitches,
  readWifiData,
  writetoRelays,
  writetoRF,
};
//state initialization
StructState machine_state = readWifiData;

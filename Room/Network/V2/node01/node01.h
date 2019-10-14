#define testenable 0

/*Global variables*/
extern boolean SwitchRead_Buffer[];

/*local variables*/
enum StructState
{
  readStorage,
  readSwitches,
  readWifiData,
  writetoRelays,
  writetoRF,
};
//state initialization
StructState machine_state = readStorage;

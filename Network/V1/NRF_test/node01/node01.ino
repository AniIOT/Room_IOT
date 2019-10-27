#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

#define ce  9  //define your ce pin here
#define csn 10 //define your csn pin here

RF24 radio(ce, csn);
RF24Network network(radio);

const uint16_t this_node = 01;
int Data = 0;

void setup() 
{
  radio.begin();
  network.begin(90, this_node);
}

void loop() 
{
  Data = rf01(); //this is your received data
  delay(5); //might need to change this if you see glitches in comm.
}

int rf01()
{
  int incomingData;
  network.update();
  if ( network.available() )
  {
    RF24NetworkHeader header;
    network.read(header, &incomingData, sizeof(incomingData));
  }
  return incomingData;
}

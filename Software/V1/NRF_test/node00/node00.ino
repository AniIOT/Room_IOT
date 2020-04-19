#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

#define ce  9  //define your ce pin here
#define csn 10 //define your csn pin here

RF24 radio(ce, csn);
RF24Network network(radio);

const uint16_t this_node = 00;
const uint16_t node01 = 01;

void setup() 
{
 radio.begin();
 network.begin(90, this_node);
}

void loop() 
{
  rf00(/*put your int or long value for sending here*/);
  delay(5); //might need to change this if you see glitches in comm.
}

void rf00(unsigned long Value)
{
  network.update();
  RF24NetworkHeader header(node01);
  bool ok = network.write(header, &Value, sizeof(Value));
}

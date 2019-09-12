#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

#define sw1 8
#define sw2 7
#define sw3 6
#define sw4 5
#define sw5 4
#define sw6 3
#define r1 19
#define r2 18
#define r3 17
#define r4 16
#define r5 15
#define r6 14

void rf01();
void decoder();
void pin_setup();

RF24 radio(9, 10);
RF24Network network(radio);
const uint16_t this_node = 01;

int rw1 = 0, rw2 = 0, rw3 = 0, rw4 = 0, rw5 = 0, rw6 = 0;
int s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0, s6 = 0;
int a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0;
int pa1 = 0, pa2 = 0, pa3 = 0, pa4 = 0, pa5 = 0, pa6 = 0;
int ps1 = 0, ps2 = 0, ps3 = 0, ps4 = 0, ps5 = 0, ps6 = 0;
int incomingData;

void setup()
{
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  pinMode(sw3, INPUT);
  pinMode(sw4, INPUT);
  pinMode(sw5, INPUT);
  pinMode(sw6, INPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  pinMode(r5, OUTPUT);
  pinMode(r6, OUTPUT);
  
  radio.begin();
  network.begin(90, this_node);

  pin_setup();
  Serial.begin(57600);
}

void loop()
{ 
  a1 = (s1) xor rw1;
  a2 = (s2) xor rw2;
  a3 = (s3) xor rw3;
  a4 = (s4) xor rw4;
  a5 = (s5) xor rw5;
  a6 = (s6) xor rw6;
  
  rf01();
  
  s1 = digitalRead(sw1);
  if ( s1 != ps1)
  {
    delayMicroseconds(200);
    s1 = digitalRead(sw1);
  }
  s2 = digitalRead(sw2);
  if ( s2 != ps2)
  {
    delayMicroseconds(200);
    s2 = digitalRead(sw2);
  }
  s3 = digitalRead(sw3);
  if ( s3 != ps3)
  {
    delayMicroseconds(200);
    s3 = digitalRead(sw3);
  }
  s4 = digitalRead(sw4);
  if ( s4 != ps4)
  {
    delayMicroseconds(200);
    s4 = digitalRead(sw4);
  }
  s5 = digitalRead(sw5);
  if ( s5 != ps5)
  {
    delayMicroseconds(200);
    s5 = digitalRead(sw5);
  }
  s6 = digitalRead(sw6);
  if ( s6 != ps6)
  {
    delayMicroseconds(200);
    s6 = digitalRead(sw6);
  }
  
  if ( a1 != pa1)
    digitalWrite(r1, a1);
  if ( a2 != pa2)
    digitalWrite(r2, a2);
  if ( a3 != pa3)
    digitalWrite(r3, a3);
  if ( a4 != pa4)
    digitalWrite(r6, a4);
  if ( a5 != pa5)
    digitalWrite(r5, a5);
  if ( a6 != pa6)
    digitalWrite(r4, a6);
    
  pa1 = a1;
  pa2 = a2;
  pa3 = a3;
  pa4 = a4;
  pa5 = a5;
  pa6 = a6;
  ps1 = s1;
  ps2 = s2;
  ps3 = s3;
  ps4 = s4;
  ps5 = s5;
  ps6 = s6;

  Serial.print("Switches: ");
  Serial.print(a1);
  Serial.print(a2);
  Serial.print(a3);
  Serial.print(a4);
  Serial.print(a5);
  Serial.println(a6);
}

#include <SoftwareSerial.h>
SoftwareSerial eSerial(4, 5);

#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>

#define s1 6
#define s2 7
#define s3 8
#define s0 0
#define r1  2
#define r2  3
#define r3  19
#define r4  18
#define r5  17
#define r6  16
#define r7  15
#define r8  14
#define ce  9
#define csn 10

RF24 radio(ce, csn);
RF24Network network(radio);

void Sample();
void VoiceCom();
void rf00();
void pin_setup();
void mem_read();
void mem_write();

char dat[4], s;
int received = 0;
int com, buff;
int ct = 0, pt = 0, tt = 0;
bool nextByteFlag = false;

int sw1 = 0, sw2 = 0, sw3 = 0, sw4 = 0, sw5 = 0, sw6 = 0, sw7 = 0, sw8 = 0;
int w1 = 0, w2 = 0, w3 = 0, w4 = 0, w5 = 0, w6 = 0, w7 = 0, w8 = 0;
int pa1 = 0, pa2 = 0, pa3 = 0, pa4 = 0, pa5 = 0, pa6 = 0, pa7 = 0, pa8 = 0;
int a1 = 0, a2 = 0, a3 = 0, a4 = 0, a5 = 0, a6 = 0, a7 = 0, a8 = 0;

const uint16_t this_node = 00;
const uint16_t node01 = 01;

void setup()
{
  pinMode(s0, INPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(r1, OUTPUT);
  pinMode(r2, OUTPUT);
  pinMode(r3, OUTPUT);
  pinMode(r4, OUTPUT);
  pinMode(r5, OUTPUT);
  pinMode(r6, OUTPUT);
  pinMode(r7, OUTPUT);
  pinMode(r8, OUTPUT);

  eSerial.begin(115200);
  dat[received] = '\0';

  radio.begin();
  network.begin(90, this_node);
//  Serial.begin(115200);

  digitalWrite(r1,a1);
  digitalWrite(r2,a2);
  digitalWrite(r3,a3);
  digitalWrite(r4,a4);
  digitalWrite(r5,a5);
  digitalWrite(r6,a6);
  digitalWrite(r7,a7);
  digitalWrite(r8,a8);
  pin_setup();
}

void loop()
{
  VoiceCom();
  Sample();

  a1 = w1 xor sw1;
  a2 = w2 xor sw2;
  a3 = w3 xor sw3;
  a4 = w4 xor sw4;
  a5 = w5 xor sw5;
  a6 = w6 xor sw6;
  a7 = w7 xor sw7;
  a8 = w8 xor sw8;

  if ( a1 != pa1)
    digitalWrite(r2, a1);
  if ( a2 != pa2)
    digitalWrite(r3, a2);
  if ( a3 != pa3)
    digitalWrite(r4, a3);
  if ( a4 != pa4)
    digitalWrite(r5, a4);
  if ( a5 != pa5)
    digitalWrite(r6, a5);
  if ( a6 != pa6)
    digitalWrite(r7, a6);
  if ( a7 != pa7)
    digitalWrite(r1, a7);
  if ( a8 != pa8)
    digitalWrite(r8, a8);

  pa1 = a1;
  pa2 = a2;
  pa3 = a3;
  pa4 = a4;
  pa5 = a5;
  pa6 = a6;
  pa7 = a7;
  pa8 = a8;
}

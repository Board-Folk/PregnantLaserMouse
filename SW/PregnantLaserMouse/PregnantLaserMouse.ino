/*
 * Pregnant Laser Mouse
 * By Chrissy
 * Version 1.0
 * Copyright 2024
 * 
 * ADNS9800 interface based on code from https://github.com/mrjohnk/ADNS-9800
 * QUADRATURE interface based on blog from http://danceswithferrets.org/geekblog/?p=742
 * 
 * 
 * !!!!! WARNING !!!!!
 * 
 * Don't Plug in the USB to the Arduino while connected to the Amiga/Computer, as this will join the 5V together and may damage something.
 * 
 * !!!!! WARNING !!!!!
 * 
 */

#include <SPI.h>
#include <SPI.h>
#define PIN_NCS    7
#define PIN_MOTION 6

//AMIGA PIN OUT
#define PIN_XB   2 //AMIGA PIN 4
#define PIN_XA   3 //AMIGA PIN 5
#define PIN_YB   4 //AMIGA PIN 6
#define PIN_YA   5 //AMIGA PIN 7

//MOTION SPEED CONTROL
//#define FAST //UNCOMMENT TO MAKE AS FAST AS POSSIBLE
unsigned long delayX = 0;
unsigned long delayY = 0;
const unsigned long delayBy = 100; //ALTER THIS TO CHANGE MOUSE SPEED, HIGHER NUMBER SLOWER

//QUADRATURE INDEXING (PULSES)
const bool chan1[] = {HIGH, HIGH, LOW, LOW};
const bool chan2[] = {HIGH, LOW, LOW, HIGH};
signed int xIndex, yIndex = 0;

void setup() {
  delay(2000);
  Serial.begin(9600);
  setupSPI();
  if (setupADNS()){
    setupQuadrature();
    #ifdef FAST
      attachInterrupt(PIN_MOTION, readMotion, FALLING); //Not sure to have this, or just read in loop
    #endif
  }
}

void setupSPI(){
  Serial.println("Init SPI");
  pinMode(PIN_NCS, OUTPUT);
  pinMode(PIN_MOTION, INPUT_PULLUP); 
  //digitalWrite(PIN_NCS, HIGH); // disable Slave Select

  SPI.begin();
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  delay(100);
}

//SETUP THE QUADRATURE
void setupQuadrature() {
  Serial.println("Init Quadrature");
  pinMode(PIN_XA, OUTPUT);
  pinMode(PIN_XB, OUTPUT);
  pinMode(PIN_YA, OUTPUT);
  pinMode(PIN_YB, OUTPUT);
  digitalWrite(PIN_XA, HIGH);
  digitalWrite(PIN_XB, HIGH);
  digitalWrite(PIN_YA, HIGH);
  digitalWrite(PIN_YB, HIGH);
}

void loop() {
  #ifndef FAST
    readMotion();
  #endif
}


//READ THE MOUSE MOTION ON HOW FAR IT HAS CURRENTLY TRAVELLED AND PULSE THE AMIGA THE DISTANCE
void readMotion(){
  volatile int* xydat = readADNS();
  signed int mX = convTwosComp(xydat[1]);
  signed int mY = convTwosComp(xydat[3]);
  pulse(mX, mY);
}

//PULSE THE AMIGA
void pulse(signed int mX, signed int mY){
  bool rX = (mX <= 0); //Check Reverse
  bool rY = (mY <= 0); //Check Reverse

  mX = abs(mX); //Make it positive
  mY = abs(mY); //Make it positive

  while (mX || mY) { //May Need Timer Delays ??????
    if (mX > 0 && doX()){
      xIndex = (rX ? xIndex + 1 : xIndex - 1);
      if (xIndex < 0) xIndex = 3; else if (xIndex > 3) xIndex = 0;
      mX--;
      pulseX(xIndex);
    }

    if (mY > 0 && doY()){
      yIndex = rY ? yIndex + 1 : yIndex - 1;
      if (yIndex < 0) yIndex = 3; else if (yIndex > 3) yIndex = 0;
      mY--;
      pulseY(yIndex);
    } 
  }
  readADNS(); //Clear ADNS Buffer ready for new read
}

//PULSE THE QUADRATURE X
void pulseX(signed int index) {
  digitalWrite(PIN_XA, chan1[index]);
  digitalWrite(PIN_XB, chan2[index]);
}

//PULSE THE QUADRATURE Y
void pulseY(signed int index) {
  digitalWrite(PIN_YA, chan1[index]);
  digitalWrite(PIN_YB, chan2[index]);
}

//UTILS
int convTwosComp(int b){
  if(b & 0x80){
    b = -1 * ((b ^ 0xff) + 1);
  }
  return b;
}

bool doX(){
  #ifdef FAST
    return true;
  #else
    if ((micros() > delayX)){
      delayX = micros() + delayBy;
      return true;
    }
    return false;
  #endif
}

bool doY(){
  #ifdef FAST
    return true;
  #else
    if ((micros() > delayY)){
      delayY = micros() + delayBy;
      return true;
    }
    return false;
  #endif
}

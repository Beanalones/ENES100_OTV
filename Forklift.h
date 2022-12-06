#ifndef FORKLIFT_h
#define FORKLIFT_h
#include "Pinout.h"
#include "Arduino.h"

#define dcTol 3
#define hallSensorConstant 100

void setupForklift();

void forkliftUp();
void forkliftDown();
void forkliftStop();

float pulseLow();
float pulseHigh();
bool readDataConnection(int &dutyCycle);
bool isMagnetic();

void testLimitSwitch();
void testHallEffect();
void testForkliftMovement();
void testForkliftProbe();

#endif
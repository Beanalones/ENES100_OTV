#ifndef PINOUT_H
#define PINOUT_H
#include "Arduino.h"

#define wifiTXPin 11
#define wifiRXPin 12

//Standard PWM DC control
#define EnLeft 6     //M1 Speed Control, left
#define In1Left 5    //M1 Direction Control
#define In2Left 7    //M1 Direction Control

#define EnRight 3     //M2 Speed Control, right
#define In1Right 4    //M1 Direction Control
#define In2Right 2    //M1 Direction Control

#define data A0
#define hallSensor A1

// Forlift Motor Connections
//Change this if you wish to use another diagram
#define EnA 10
#define In1 9
#define In2 8

#define limitUp A4
#define limitDown A5

#define echoPin A2
#define trigPin A3 

#endif
#ifndef DRIVE_h
#define DRIVE_h
#include "Pinout.h"

void setupDrive();
// -127, 127  Negative is backward
void drive(int vel);

// -127, 127  Negative is CW
void turn(int vel);

void brake();
void coast();

void leftForward(char speed);
void leftBackward(char speed);

void rightForward(char speed);
void rightBackward(char speed);

#endif
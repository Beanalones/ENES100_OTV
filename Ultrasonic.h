#ifndef ULTRASONIC_h
#define ULTRASONIC_h
#include "Pinout.h"

// If a distance greater than this is read, the OTV will
// assume no obstacle is present
#define distanceConstant 80

void setupUltrasonic();
double readUltra();
bool facingObstacle();
void testUltra();

#endif
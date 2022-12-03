#ifndef NAVIGATION_h
#define NAVIGATION_h
#include "Pinout.h"

enum CenterPoint {
    Aruco, Forklift, Center
};

#define minWheelSpeed 200
#define maxWheelSpeed 255

// The offset from the aruco to the center of the arm
#define forkliftXOff 0.075
#define forkliftYOff 0.28

// The offset from the aruco to the center of the robot
// The robot center is the point the wheels rotate around
#define centerXOff 0.2
#define centerYOff 0.2

void moveToPoint(double xTarget, double yTarget, CenterPoint center);
void turnToAngle(double theta);

double map_value(double inVal, double inMin, double inMax, double outMin, double outMax);

void getForkliftPos(double x, double y, double theta, double &xOut, double &yOut);
void getCenterPos(double x, double y, double theta, double &xOut, double &yOut);

// Returns the absolute angle to the mission site
double angleToPoint(double xi, double yi, double xf, double yf);

// Returns the smallest difference between two angles, -pi to pi
double getDeltaAngle(double initialAngle, double finalAngle);

// Returns the speed the wheels should turn for a specific angle
// INPUT: -pi, pi
// OUTPUT: -127, 127
double turnSpeedForDeltaTheta(double dTheta);

// Returns the speed the wheels should turn for a specific distance
// INPUT: -intMax to intMax
// OUTPUT: -127, 127
double driveSpeedForDistance(double distance);

void testPosOffsets();

#endif
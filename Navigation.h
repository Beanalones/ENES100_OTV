#ifndef NAVIGATION_h
#define NAVIGATION_h
#include "Pinout.h"

enum CenterPoint {
    Aruco, Forklift, Center
};

#define minTurnSpeed 200
#define minDriveSpeed 100

// The offset from the aruco to the center of the arm
#define forkliftXOff 0.09
#define forkliftYOff 0.28

// The offset from the aruco to the center of the robot
// The robot center is the point the wheels rotate around
#define centerXOff 0.075
#define centerYOff 0.11

void moveToPoint(double xTarget, double yTarget, CenterPoint center);
void turnToAngle(double theta);

void getForkliftPos(double x, double y, double theta, double &xOut, double &yOut);
void getCenterPos(double x, double y, double theta, double &xOut, double &yOut);

// Returns the absolute angle to the mission site
double angleToPoint(double xi, double yi, double xf, double yf);

// Returns the smallest difference between two angles, -pi to pi
double getDeltaAngle(double initialAngle, double finalAngle);

void testPosOffsets();

#endif
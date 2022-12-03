#include "Arduino.h"
#include "Drive.h"
#include "Navigation.h"
#include "Enes100.h"

void moveToPoint(double xTarget, double yTarget, CenterPoint center) {
    double distance = 100;
    double lastAngle = NAN;
    long int lastTurnTime = 0;
    while (distance >= 0.02) {
        Enes100.updateLocation();
        
        double x, y;
        if (center = Forklift) {
            getForkliftPos(Enes100.location.x, Enes100.location.y, Enes100.location.theta, x, y);
        }
        else if (center = Center) {
            getCenterPos(Enes100.location.x, Enes100.location.y, Enes100.location.theta, x, y);
        }
        else {
            x = Enes100.location.x;
            y = Enes100.location.y;
        }
        double ang = angleToPoint(x, y, xTarget, yTarget);
        double dTheta = getDeltaAngle(Enes100.location.theta, ang);

        if ((!isnan(lastAngle) && (lastAngle * dTheta > 0)) || (abs(dTheta) > 0.3) && (distance > 0.4) && (millis() - lastTurnTime > 2500) ) {
            turn(turnSpeedForDeltaTheta(dTheta));
            lastAngle = dTheta;
            lastTurnTime = millis();
        }
        else {
            distance = sqrt(pow(xTarget - x, 2.) + pow(yTarget - y, 2.));
            if(dTheta > (M_PI / 2.))
              break;
            else
              drive(driveSpeedForDistance(distance));
            
            lastAngle = NAN;
        }
    Enes100.print("X: ");
    Enes100.print(Enes100.location.x);
    Enes100.print("    Y: ");
    Enes100.print(Enes100.location.y);
    Enes100.print("    Theta: ");
    Enes100.print(Enes100.location.theta);
    Enes100.print("    Angle: ");
    Enes100.print(ang);
        Enes100.print("     Distance: ");
        Enes100.print(distance);
        Enes100.print("    dtheta: ");
        Enes100.println(dTheta);
    }
    stop();
}

void turnToAngle(double theta) {
    double lastAngle = NAN;
    while (1) {
        Enes100.updateLocation();
        double dTheta = getDeltaAngle(Enes100.location.theta, theta);
        Enes100.print(lastAngle);
        Enes100.print("   ");
        Enes100.println(dTheta);
        if ((isnan(lastAngle) || (lastAngle * dTheta >= 0))) {
            turn(turnSpeedForDeltaTheta(dTheta));
            
            lastAngle = dTheta;
        }
        else
            break;
    }
    stop();
    
}

double map_value(double inVal, double inMin, double inMax, double outMin, double outMax)
{
  double inValNorm = inVal - inMin;
  double aUpperNorm = inMax - inMin;
  double normPosition = inValNorm / aUpperNorm;

  double bUpperNorm = outMax - outMin;
  double bValNorm = normPosition * bUpperNorm;
  double outVal = outMin + bValNorm;

  return outVal;
}

void getForkliftPos(double x, double y, double theta, double &xOut, double &yOut){
	xOut = x + forkliftYOff*cos(theta) + forkliftXOff*sin(theta);
	yOut = y + forkliftYOff*sin(theta) + forkliftXOff*cos(theta);
}

void getCenterPos(double x, double y, double theta, double &xOut, double &yOut){
  xOut = x + centerYOff*cos(theta) + centerXOff*sin(theta);
  yOut = y + centerYOff*sin(theta) + centerXOff*cos(theta);
}


double angleToPoint(double xi, double yi, double xf, double yf){
  double delta_x = xf - xi;
  double delta_y = yf - yi;
  double theta_radians = atan2(delta_y, delta_x);

  return theta_radians;
}
double getDeltaAngle(double initialAngle, double finalAngle){
  double delta = finalAngle - initialAngle;
  if(delta > M_PI)
    delta -= 2. * M_PI;
  else if (delta < -M_PI)
    delta += 2. * M_PI;
  return delta;
}
double turnSpeedForDeltaTheta(double dTheta){
  //int value = (int) map_value(dTheta, -M_PI, M_PI, -127, 127 );
  //int value = ((double)maxWheelSpeed - (double)minWheelSpeed) / sqrt(M_PI) * sqrt(abs(dTheta)) + (double)minWheelSpeed;
  int value = map_value(abs(dTheta), 0., M_PI, minWheelSpeed, maxWheelSpeed);
  value=200;
  value = copysign(value, dTheta);

  return value;
}

double driveSpeedForDistance(double distance){
  //int value = ((double)maxWheelSpeed - (double)minWheelSpeed) / sqrt(2.) * sqrt(abs(distance)) + (double)minWheelSpeed;
  int value = map_value(abs(distance), 0., 2., minWheelSpeed, maxWheelSpeed);
  value=100;
  value = copysign(value, distance);
  return value;
}
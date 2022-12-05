#include "Arduino.h"
#include "Drive.h"
#include "Navigation.h"
#include "Enes100.h"
#include <Vector.h>

void moveToPoint(double xTarget, double yTarget, CenterPoint center) {
    double distance = 100;
    double initialDistance = NAN;
    double lastAngle = NAN;
    long int lastTurnTime = -5000;

    double storageX_array[10] = {0,0,0,0,0,0,0,0,0,0};
    double storageY_array[10] = {0,0,0,0,0,0,0,0,0,0};
    double storageTheta_array[10] = {0,0,0,0,0,0,0,0,0,0};
    Vector<double> oldX;
    Vector<double> oldY;
    Vector<double> oldTheta;
    oldX.setStorage(storageX_array);
    oldY.setStorage(storageY_array);
    oldTheta.setStorage(storageTheta_array);
    long int lastOldUpdate = -5000;

    while (1) {
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
        if(isnan(initialDistance))
          initialDistance = sqrt(pow(xTarget - x, 2.) + pow(yTarget - y, 2.));

        double ang = angleToPoint(x, y, xTarget, yTarget);
        double dTheta = getDeltaAngle(Enes100.location.theta, ang);

        // Turn if we were just doing a turn, that we have not finished, or
        // We have come off course
        if ((!isnan(lastAngle) && (lastAngle * dTheta >= 0.)) || 
            (abs(dTheta) > 0.3) && (distance > (initialDistance / 2.)) && (millis() - lastTurnTime > 2500) ) {

            // Gun it in case we get stuck, this should never happen
            if((oldTheta.size() > 9) && (abs(Enes100.location.theta - oldTheta[0]) < 0.02)){
              turn(copysign(255, dTheta));
            }
            // We should always be turning as slow as possible
            else{
              turn(copysign(minTurnSpeed, dTheta));
            }
            lastAngle = dTheta;
            lastTurnTime = millis();
            oldX.clear();
            oldY.clear();
        }
        else {
            distance = sqrt(pow(xTarget - x, 2.) + pow(yTarget - y, 2.));
            // If the angle became large, we past the point. Stop
            // Else keep driving
            if(dTheta > (M_PI / 2.))
              break;
            else{
              // Give an extra boost if we get stuck on the obstacles
              if((oldX.size() > 9) && (sqrt(pow(oldX[0] - x,2) + pow(oldY[0] - y,2)) < 0.02)){
                drive(255);
              }
              
              else{
                drive(minDriveSpeed);
              }
              
            }
            lastAngle = NAN;
            oldTheta.clear();
        }

        // update the old locations list
        if(millis() - lastOldUpdate > 200){
          if(oldX.size() > 9){
            oldY.remove(0);
            oldX.remove(0);
            oldTheta.remove(0);
          }
          oldY.push_back(y);
          oldX.push_back(x);
          oldTheta.push_back(Enes100.location.theta);
          lastOldUpdate = millis();
        }

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
            turn(copysign(minTurnSpeed, dTheta));
            
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
// double turnSpeedForDeltaTheta(double dTheta){
//   //int value = (int) map_value(dTheta, -M_PI, M_PI, -127, 127 );
//   //int value = ((double)maxWheelSpeed - (double)minWheelSpeed) / sqrt(M_PI) * sqrt(abs(dTheta)) + (double)minWheelSpeed;
//   int value = map_value(abs(dTheta), 0., M_PI, minWheelSpeed, maxWheelSpeed);
//   value=200;
//   value = copysign(value, dTheta);

//   return value;
// }

// double driveSpeedForDistance(double distance){
//   //int value = ((double)maxWheelSpeed - (double)minWheelSpeed) / sqrt(2.) * sqrt(abs(distance)) + (double)minWheelSpeed;
//   int value = map_value(abs(distance), 0., 2., minWheelSpeed, maxWheelSpeed);
//   value=100;
//   value = copysign(value, distance);
//   return value;
// }
#include "Arduino.h"
#include "Drive.h"
#include "Navigation.h"
#include "Enes100.h"
#include <Vector.h>


void moveToPoint(double xTarget, double yTarget, CenterPoint center) {
    // Stuff for determining if we are stuck
    double storageX_array[10] = {0,0,0,0,0,0,0,0,0,0};
    double storageY_array[10] = {0,0,0,0,0,0,0,0,0,0};
    Vector<double> oldX;
    Vector<double> oldY;
    oldX.setStorage(storageX_array);
    oldY.setStorage(storageY_array);
    long int lastOldUpdate = -5000;

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

    turnToAngle(angleToPoint(x, y, xTarget, yTarget));
    while (1) {
        Enes100.updateLocation();
        
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

        // If the angle became large, we past the point. Stop
        // Else keep driving
        if(abs(dTheta) > (M_PI / 2.))
          break;
        else{
          // Give an extra boost if we get stuck on the obstacles
          if((oldX.size() > 9) && (sqrt(pow(oldX[0] - x,2) + pow(oldY[0] - y,2)) < 0.02))
            drive(225);
          
          else
            drive(minDriveSpeed);
          
        }

        // update the old locations list
        if(millis() - lastOldUpdate > 200){
          if(oldX.size() > 9){
            oldY.remove(0);
            oldX.remove(0);
          }
          oldY.push_back(y);
          oldX.push_back(x);
          lastOldUpdate = millis();
        }

    } // end while(1)
    brake();
}

void turnToAngle(double theta) {
    double lastAngle = NAN;
    unsigned long lastTime = 0;
    unsigned long lastOldUpdate = 0;
    double storageTheta_array[10] = {0,0,0,0,0,0,0,0,0,0};
    Vector<double> oldTheta;
    oldTheta.setStorage(storageTheta_array);
    
    while (1) {
        // Update the difference between current and target
        Enes100.updateLocation();
        double dTheta = getDeltaAngle(Enes100.location.theta, theta);

        // Do some dead reckoning
        unsigned long currentTime = millis();
        double dt = double(currentTime - lastTime) * 1000.;
        double speed = 0.;
        if(lastTime != 0){
          speed = (lastAngle - dTheta) / dt;
        }
        double deadReckonPos = (speed * dt) + dTheta;
        lastTime = currentTime;

        // Determine wheather we need to keep spinning
        if ((isnan(lastAngle) || (lastAngle * dTheta >= 0.)) && (deadReckonPos * dTheta >= 0.)) {
            // Gun it in case we get stuck, this should never happen
            if((oldTheta.size() > 9) && (abs(Enes100.location.theta - oldTheta[0]) < 0.02))
              turn(copysign(207, dTheta));
            
            // We should always be turning as slow as possible
            else
              turn(copysign(minTurnSpeed, dTheta));
            
            lastAngle = dTheta;
        }
        else
            break;

        // update the old locations list
        if(currentTime - lastOldUpdate > 200){
          if(oldTheta.size() > 9)
            oldTheta.remove(0);

          oldTheta.push_back(Enes100.location.theta);
          lastOldUpdate = millis();
        }
    }
    brake();
    
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
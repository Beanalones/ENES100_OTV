#include "Drive.h"
#include "Forklift.h"
#include "Navigation.h"
#include "Enes100.h"
#include "Ultrasonic.h"
#include "Test.h"

#define arucoID 214

void setup() {
    Serial.begin(9600);
    Serial.println("Initialized");
    Enes100.begin("Julius", DATA, arucoID, wifiTXPin, wifiRXPin);
    setupForklift();
    setupUltrasonic();
    setupDrive();

    //fullSystemTest();
    //moveToPoint(Enes100.missionSite.x, Enes100.missionSite.y, Forklift);
    //moveToPoint(2.5, 1.5, Forklift);
    //turnToAngle(0);
    Enes100.updateLocation();
    delay(2000);
    Enes100.print("Mission site is at    X: ");
    Enes100.print(Enes100.missionSite.x);
    Enes100.print("    Y: ");
    Enes100.println(Enes100.missionSite.y);
    runFullMission();
}

void loop() {}


// For these the obstacles,
// -1 means unknown
// 0 means no obstacle
// 1 means there is an obstacle 
int leftBottomObs = -1;
int leftMidObs = -1;
int leftTopObs = -1;

int rightBottomObs = -1;
int rightMidObs = -1;
int rightTopObs = -1;

void runFullMission() {
    // Move to the mission site
    //
    moveToPoint(Enes100.missionSite.x, Enes100.missionSite.y, Forklift);

    // Dock with pylon and read data
    //
    // Step 1: Put the forklift down
    forkliftDown();
    long int start = millis();
    while (digitalRead(limitDown) && (millis() - start < 250))
        delay(1);
    forkliftStop();

    drive(-255);
    delay(100);
    brake();
    // Step 2: Move back until a connection is made
    int dutyCycle = 0;
    for(int i=0;i<7;i++){
      Enes100.print("Attempt ");
      Enes100.print(i + 1);
      Enes100.print("/10");
      if(readDataConnection(dutyCycle)){
        Enes100.println("   Connection is good");
        break;
      }
      else{
        Enes100.println("  Poor connection");
      }
    }

    // Step 3: Print results
    Enes100.print("Duty cycle is ");
    Enes100.println(dutyCycle);
    Enes100.mission(CYCLE, dutyCycle);
    if (isMagnetic()) {
        Serial.println("Puck is magnetic");
        Enes100.mission(MAGNETISM, MAGNETIC);
    }
    else {
        Serial.println("Puck is not magnetic");
        Enes100.mission(MAGNETISM, NOT_MAGNETIC);
    }

    // Step 4: Put forklift up and move back some
    forkliftUp();
    while (digitalRead(limitUp))
        delay(10);
    forkliftStop();
    drive(-150);
    delay(1000);
    brake();
    // Done with data section!

    // Extration!
    //
    // Move to the bottom row and face the potential obstacle
    moveToPoint(1., 0.5, Center);
    turnToAngle(0.);
    leftBottomObs = facingObstacle();

    if (leftBottomObs) {
        // If there is an obstacle at the bottom, move to the middle
        moveToPoint(1., 1., Center);
        turnToAngle(0.);
        leftMidObs = facingObstacle();

        // If there is an obstacle at the middle, move to the top
        if (leftMidObs) {
            moveToPoint(1., 1.5, Center);
            turnToAngle(0.);
            leftTopObs = facingObstacle();
            if (leftTopObs) {
                // There should never be 2 solid obstacles
                Enes100.println("Error: Invalid obstacle setup");
            }
            moveToPoint(2., 1.5, Center);
        }
        else {
            moveToPoint(2., 1.5, Center);
            rightMidObs = facingObstacle();
        }
    }
    else {
        moveToPoint(2., 0.5, Center);
        rightBottomObs = facingObstacle();
    }

    if (facingObstacle()) {
        // If we hit an obstacle in the second column

        if (leftMidObs == 1) {
            // There should never be 2 solid obstacles
            Enes100.println("Error: Invalid obstacle setup");
        }
        else if (leftBottomObs == 1) {
            // We are at the middle column and have found 2 obstacles
            // Go to the bottom position because there cannot be an obstace there.
            moveToPoint(1.9, 0.5, Center);
        }
        else {
            // We have found only 1 obstacle, need to check other places now
            moveToPoint(1.9, 1., Center);
            if (facingObstacle) {
                moveToPoint(1.9, 1.5, Center);
                if (facingObstacle()) {
                    // There should never be 2 solid obstacles
                    Enes100.println("Error: Invalid obstacle setup");
                }
            }
        }
    }

    // We are clear of obstacles in the second column now, move forward
    moveToPoint(2.8, Enes100.location.y, Center);

    // Move in front of the log
    moveToPoint(2.8, 0.5, Center);
    turnToAngle(M_PI);

    // Clib the Log
    while (Enes100.location.x < 3.7) {
        Enes100.updateLocation();
        drive(-255);
    }
    brake();

    Enes100.println("YAAAY! Job Done!");
}
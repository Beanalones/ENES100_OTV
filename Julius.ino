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
    //drive(255);
    // moveToPoint(Enes100.missionSite.x, Enes100.missionSite.y, Forklift);
    //moveToPoint(2.5, 1.5, Forklift);
    Enes100.print("X: ");
    Enes100.print(Enes100.missionSite.x);
    Enes100.print("    Y: ");
    Enes100.println(Enes100.missionSite.y);
    //runFullMission();
    turnToAngle(0);

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
    while (digitalRead(limitDown))
        delay(1);
    forkliftStop();

    // Step 2: Move back until a connection is made
    int dutyCycle;
    // while (!readDataConnection(dutyCycle)) {
    //     drive(-100);
    //     delay(100);
    //     stop();
    // }

    // Step 3: Print results
    Enes100.print("Duty cycle is ");
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
    drive(-100);
    delay(500);
    stop();
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
            moveToPoint(1.9, 1.5, Center);
        }
        else {
            moveToPoint(1.9, 1.5, Center);
            rightMidObs = facingObstacle();
        }
    }
    else {
        moveToPoint(1.9, 1., Center);
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
    turnToAngle(0);

    // Clib the Log
    while (Enes100.location.x < 4.7) {
        Enes100.updateLocation();
        drive(255);
    }
    stop();

    Enes100.println("YAAAY! Job Done!");
    while (1) {}
}
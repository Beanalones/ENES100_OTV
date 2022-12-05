#include "Arduino.h"
#include "Test.h"

void testOffsets() {
    Enes100.updateLocation();
    double x, y;
    getForkliftPos(Enes100.location.x, Enes100.location.y, Enes100.location.theta, x, y);
    Enes100.print("ForklfitPos: ");
    Enes100.print(x);
    Enes100.print(", ");
    Enes100.println(y);

    getCenterPos(Enes100.location.x, Enes100.location.y, Enes100.location.theta, x, y);
    Enes100.print("CenterPos: ");
    Enes100.print(x);
    Enes100.print(", ");
    Enes100.println(y);

    x = Enes100.location.x;
    y = Enes100.location.y;
    Enes100.print("ArucoPos: ");
    Enes100.print(x);
    Enes100.print(", ");
    Enes100.println(y);
    delay(2000);
}

void testGetData() {
    // Dock with pylon and read data
    //
    // Step 1: Put the forklift down
    forkliftDown();
    while (digitalRead(limitDown))
        delay(50);
    forkliftStop();

    // Step 2: Move back until a connection is made
    int dutyCycle;
    while (!readDataConnection(dutyCycle)) {
        drive(-100);
        delay(100);
        brake();
    }

    // Step 3: Print results
    Enes100.print("Duty cycle is ");
    Enes100.mission(CYCLE, dutyCycle);
    if (isMagnetic()) {
        Enes100.println("Puck is magnetic");
        Enes100.mission(MAGNETISM, MAGNETIC);
    }
    else {
        Enes100.println("Puck is not magnetic");
        Enes100.mission(MAGNETISM, NOT_MAGNETIC);
    }

    // Step 4: Put forklift up and move back some
    forkliftUp();
    while (digitalRead(limitUp))
        delay(10);
    forkliftStop();
    drive(-100);
    delay(500);
    brake();
    // Done with data section!
}

void fullSystemTest() {
    Enes100.println("Testing drive forward...");
    delay(750);
    drive(255);
    delay(2000);
    brake();

    Enes100.println("Testing turn...");
    delay(750);
    turn(255);
    delay(1000);
    turn(-255);
    delay(1000);
    brake();

    // Enes100.println("Testing forklift movement...");
    // delay(750);
    // forkliftDown();
    // for (int i = 1; i < 2000; i++) {
    //     testForkliftMovement();
    // }
    // forkliftStop();

    Enes100.println("Testing ultrasonic sensor...");
    delay(750);
    for (int i = 1; i < 75; i++) {
        testUltra();
        delay(50);
    }

    Enes100.println("Testing duty cycle probe...");
    delay(750);
    long int start = millis();
    while (millis() - start < 7000) {
        testForkliftProbe();
        delay(50);
    }

    Enes100.println("Testing aruco...");
    delay(750);
    for (int i = 1; i < 10; i++) {
        testGetLocation();
        delay(50);
    }
}

void testUltra(){
  Enes100.print("Distance: ");
  Enes100.print(readUltra());
  Enes100.println(" cm");
}

void testHallEffect(){
  int val = analogRead(hallSensor);
  if(val < 70){
    Enes100.println("Magnetic");
  }
  else{
    Enes100.println("Not Magnetic");
  }
}

void testLimitSwitch(){
  Enes100.print(digitalRead(limitDown));
  Enes100.print("  ");
  Enes100.println(digitalRead(limitUp));
}
void testForkliftMovement(){
  if(!digitalRead(limitDown))
    forkliftUp();
  else if(!digitalRead(limitUp))
    forkliftDown();
  delay(1);
}

void testForkliftProbe(){
  int dutyCycle;
  if(readDataConnection(dutyCycle)){
    Enes100.print("Duty cycle: ");
    Enes100.println(dutyCycle);
    
    if(isMagnetic()){
      Enes100.println("Puck is magnetic");
    }
    else{
      Enes100.println("Puck is not magnetic");
    }
  }
  else{
    Enes100.println("Bad forklift connection");
  }
}

void testGetLocation(){
    Enes100.updateLocation();
    Enes100.print("X: ");
    Enes100.print(Enes100.location.x);
    Enes100.print("    Y: ");
    Enes100.print(Enes100.location.y);
    Enes100.print("    Theta: ");
    Enes100.println(Enes100.location.theta);
}

void testClimbLog(){
    while(Enes100.location.x < 4.7){
      Enes100.updateLocation();
      drive(255);
    }
    brake();
}
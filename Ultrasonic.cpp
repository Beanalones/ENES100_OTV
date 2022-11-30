#include "Arduino.h"
#include "Ultrasonic.h"

void setupUltrasonic(){
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
}

double readUltra(){
// Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echoPin, HIGH);
  // Calculate the distance (in cm)
  return duration * 0.034 / 2.; // Speed of sound wave divided by 2 (go and back)
}
bool facingObstacle(){
  double distance = readUltra();
  
  if(distance > distanceConstant)
    return false;
  else
    return true;
}

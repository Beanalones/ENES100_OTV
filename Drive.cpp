#include "Arduino.h"
#include "Drive.h"

void setupDrive(){
  pinMode(EnLeft, OUTPUT);
  pinMode(In1Left, OUTPUT);
  pinMode(In2Left, OUTPUT);

  pinMode(EnRight, OUTPUT);
  pinMode(In1Right, OUTPUT);
  pinMode(In2Right, OUTPUT);
  brake();
}
// -127, 127  Negative is backward
void drive(int vel){
  if(vel > 0){
    leftForward(abs(vel));
    rightForward(abs(vel));
  }
  else{
    leftBackward(abs(vel));
    rightBackward(abs(vel));
  }
}

// -127, 127  Negative is CW
void turn(int vel){
  if(vel > 0){
    leftBackward(abs(vel));
    rightForward(abs(vel));
  }
  else{
    leftForward(abs(vel));
    rightBackward(abs(vel));
  }
}

void brake(){
  analogWrite(EnLeft, 255);
  digitalWrite(In1Left,HIGH);
  digitalWrite(In2Left,HIGH);

  analogWrite(EnRight, 255);
  digitalWrite(In1Right,HIGH);
  digitalWrite(In2Right,HIGH);
}
void coast(){
  analogWrite(EnLeft, 0);
  digitalWrite(In1Left,HIGH);
  digitalWrite(In2Left,HIGH);

  analogWrite(EnRight, 0);
  digitalWrite(In1Right,HIGH);
  digitalWrite(In2Right,HIGH);
}

void leftBackward(char speed){
  analogWrite(EnLeft, speed);
  digitalWrite(In1Left,HIGH);
  digitalWrite(In2Left,LOW);
}
void leftForward(char speed){
  analogWrite(EnLeft, speed);
  digitalWrite(In1Left,LOW);
  digitalWrite(In2Left,HIGH);
}

void rightBackward(char speed){
  analogWrite(EnRight, speed);
  digitalWrite(In1Right,HIGH);
  digitalWrite(In2Right,LOW);
}
void rightForward(char speed){
  analogWrite(EnRight, speed);
  digitalWrite(In1Right,LOW);
  digitalWrite(In2Right,HIGH);
}

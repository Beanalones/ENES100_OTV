#include "Forklift.h"

void setupForklift(){
  pinMode(data, INPUT);
  pinMode(hallSensor, INPUT_PULLUP);

  pinMode(EnA, OUTPUT);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(limitDown, INPUT_PULLUP);
  pinMode(limitUp, INPUT_PULLUP);
}

void forkliftUp(){
  // turn on motor A
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  // set speed to 150 out 255
  analogWrite(EnA, 200);
}
void forkliftDown(){
    // turn on motor A
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  // set speed to 150 out 255
  analogWrite(EnA, 200);
}
void forkliftStop(){
    // turn on motor A
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
}

float pulseLow(){
  long int start = micros();
  while(analogRead(data) > 900){
    if(micros() - start > 1000000)
      return 0;
  }
  long int lowStart = micros();
  while(analogRead(data) < 900){
    if(micros() - start > 1000000)
      return 0;
  }
  return (float)(micros() - lowStart) / 1000.;
}

float pulseHigh(){
  long int start = micros();
  while(analogRead(data) < 900){
    if(micros() - start > 1000000)
      return 0;
  }
  long int lowStart = micros();
  while(analogRead(data) > 900){
    if(micros() - start > 1000000)
      return 0;
  }
  return (float)(micros() - lowStart) / 1000.;
}

bool readDataConnection(int &dutyCycle){
  float ontime = pulseHigh();
  float offtime = pulseLow();

  if((ontime == 0) || (offtime == 0))
    return false;

  float period = ontime+offtime;

  if((period > 15) || (period < 5))
    return false;
  int duty = (int)(((float)ontime/(float)period)*100.);

  if((10 - dcTol < duty) && (duty < 10 + dcTol))
    dutyCycle = 10;
  else if((30 - dcTol < duty) && (duty < 30 + dcTol))
    dutyCycle = 30;
  else if((50 - dcTol < duty) && (duty < 50 + dcTol))
    dutyCycle = 50;
  else if((70 - dcTol < duty) && (duty < 70 + dcTol))
    dutyCycle = 70;
  else if((90 - dcTol < duty) && (duty < 90 + dcTol))
    dutyCycle = 90;
  else{
    //Serial.print("Invalid duty cycle");
    //Serial.println(dutyCycle);
  }
  
  return true;
}

bool isMagnetic(){
  return  analogRead(hallSensor) < 100;
}

#include "ColorSensor.h"
ColorProfile ColorSensor::whiteProfile; 
ColorProfile ColorSensor::blackProfile;
ColorProfile ColorSensor::silverProfile;

ColorSensor::ColorSensor(){
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  digitalWrite(S0, HIGH); 
  digitalWrite(S1, LOW);
}
bool ColorSensor::withinRange(int input, int value){
  if(input + input * range > value && input - input*range < value)    
    return true;
  return false;
}

short int ColorSensor::getRedFrequency(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  return pulseIn(sensorOut, LOW);
}

short int ColorSensor::getGreenFrequency(){
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    return pulseIn(sensorOut, LOW);  
}

short int ColorSensor::getBlueFrequency(){
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    return pulseIn(sensorOut, LOW);
}

Color ColorSensor::getColor(){ 
  if(millis() - lastReadTime > timeoutReads){

    redFrequency = getRedFrequency();
    blueFrequency = getBlueFrequency();
    greenFrequency = getGreenFrequency();   

    lastReadTime = millis();
  }
  if(calibrar){
    Serial.print("R= ");
    Serial.print(redFrequency);
    Serial.print(" G= ");
    Serial.print(greenFrequency);
    Serial.print(" B= ");
    Serial.print(blueFrequency);
    Serial.print("\tWP: ");
    Serial.print(whiteProfile.r);
    Serial.print(" ");
    Serial.print(whiteProfile.g);
    Serial.print(" ");
    Serial.print(whiteProfile.b);
    Serial.print("\tBP: ");
    Serial.print(blackProfile.r);
    Serial.print(" ");
    Serial.print(blackProfile.g);
    Serial.print(" ");
    Serial.print(blackProfile.b);
    Serial.print("\tSP: ");
    Serial.print(silverProfile.r);
    Serial.print(" ");
    Serial.print(silverProfile.g);
    Serial.print(" ");
    Serial.println(silverProfile.b);
  }

  if((withinRange(redFrequency, whiteProfile.r)&& withinRange(greenFrequency, whiteProfile.g))&& withinRange(blueFrequency, whiteProfile.b)){
      if(calibrar)
        Serial.print(" WHITE ");
      return White;
    }else if(((withinRange(redFrequency, blackProfile.r)&& withinRange(greenFrequency, blackProfile.g))&& withinRange(blueFrequency, blackProfile.b)) ){
      if(calibrar)
        Serial.print(" BLACK ");
      return Black;
    }else if((withinRange(redFrequency, silverProfile.r)&& withinRange(greenFrequency, silverProfile.g))&& withinRange(blueFrequency, silverProfile.b)){
      if(calibrar)
        Serial.print(" SILVER ");
      return Silver;
    }else{
      Serial.print("NONE");
      return White;
    }
    Serial.println();
}

void ColorSensor::calibrateWhite(){
  int sumBuffer = 0;
  for(int i = 0; i < sensorReadings; ++i){
    sumBuffer += getRedFrequency();
  }

  whiteProfile.r = sumBuffer / sensorReadings;
  sumBuffer = 0;

  for(int i = 0; i < sensorReadings; ++i){
    sumBuffer += getGreenFrequency();
  }

  whiteProfile.g = sumBuffer / sensorReadings;
  sumBuffer = 0;

  for(int i = 0; i < sensorReadings; ++i){
    sumBuffer += getBlueFrequency();
  }

  whiteProfile.b = sumBuffer / sensorReadings; 

}

void ColorSensor::calibrateBlack(){
  short int sumBuffer = 0;
  for(int i = 0; i < sensorReadings; ++i){
    sumBuffer += getRedFrequency();
  }

  blackProfile.r = sumBuffer / sensorReadings;
  sumBuffer = 0;

  for(int i = 0; i < sensorReadings; ++i){
    sumBuffer += getGreenFrequency();
  }

  blackProfile.g = sumBuffer / sensorReadings;
  sumBuffer = 0;

  for(int i = 0; i < sensorReadings; ++i){
    sumBuffer += getBlueFrequency();
  }

  blackProfile.b = sumBuffer / sensorReadings; 
}

void ColorSensor::calibrateSilver(){
  short int sumBuffer = 0;
  for(int i = 0; i < sensorReadings; ++i){
    sumBuffer += getRedFrequency();
  }

  silverProfile.r = sumBuffer / sensorReadings;
  sumBuffer = 0;

  for(int i = 0; i < sensorReadings; ++i){
    sumBuffer += getGreenFrequency();
  }

  silverProfile.g = sumBuffer / sensorReadings;
  sumBuffer = 0;

  for(int i = 0; i < sensorReadings; ++i){
    sumBuffer += getBlueFrequency();
  }

  silverProfile.b = sumBuffer / sensorReadings; 

}
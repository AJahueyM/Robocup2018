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

  whiteProfile.r = EEPROM.read(whiteDirR);
  whiteProfile.g = EEPROM.read(whiteDirG);
  whiteProfile.b = EEPROM.read(whiteDirB);

  blackProfile.r = EEPROM.read(blackDirR);
  blackProfile.g = EEPROM.read(blackDirG);
  blackProfile.b = EEPROM.read(blackDirB);

 silverProfile.r = EEPROM.read(silverDirR);
 silverProfile.g = EEPROM.read(silverDirG);
 silverProfile.b = EEPROM.read(silverDirB);

}



bool ColorSensor::withinRange(int input, int value){
  if(input + input * range > value && input - input*range < value)    
    return true;
  return false;
}

uint8_t ColorSensor::getRedFrequency(){
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  int value =  pulseIn(sensorOut, LOW);
  return value > 0  && value < 254 ? value : 0;
}

uint8_t ColorSensor::getGreenFrequency(){
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    int value =  pulseIn(sensorOut, LOW);
    return value > 0 && value < 254 ? value : 0;
  }

uint8_t ColorSensor::getBlueFrequency(){
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    int value =  pulseIn(sensorOut, LOW);
    return value > 0  && value < 254? value : 0;
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
  /// REMOVE THIS
  return White;
  
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

void ColorSensor::saveProfiles(){
    EEPROM.write(this->whiteDirR,whiteProfile.r);
    EEPROM.write(this->whiteDirG,whiteProfile.g);
    EEPROM.write(this->whiteDirB,whiteProfile.b);

    EEPROM.write(this->blackDirR,blackProfile.r);
    EEPROM.write(this->blackDirG,blackProfile.g);
    EEPROM.write(this->blackDirB,blackProfile.b);

    EEPROM.write(this->silverDirR,silverProfile.r);
    EEPROM.write(this->silverDirG,silverProfile.g);
    EEPROM.write(this->silverDirB,silverProfile.b);
}
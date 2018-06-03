#include "ColorSensor.h"

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

Color ColorSensor::getColor(){ 
  if(millis() - lastReadTime > timeoutReads){
    digitalWrite(S2,LOW);
    digitalWrite(S3,LOW);
    redFrequency = pulseIn(sensorOut, LOW);
  
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    blueFrequency = pulseIn(sensorOut, LOW);

    
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    greenFrequency = pulseIn(sensorOut, LOW);    
    lastReadTime = millis();
  }
  if(calibrar){
    Serial.print("R= ");
    Serial.print(redFrequency);
    Serial.print(" G= ");
    Serial.print(greenFrequency);
    Serial.print(" B= ");
    Serial.println(blueFrequency);
  }

  if((withinRange(redFrequency, 13)&& withinRange(greenFrequency, 16))&& withinRange(blueFrequency, 5)){
      if(calibrar)
        Serial.print(" WHITE ");
      return White;
    }else if(((withinRange(redFrequency, 20)&& withinRange(greenFrequency, 25))&& withinRange(blueFrequency, 12)) ){
      if(calibrar)
        Serial.print(" BLACK ");
      return Black;
    }else if((withinRange(redFrequency, 5)&& withinRange(greenFrequency, 7))&& withinRange(blueFrequency, 2)){
      if(calibrar)
        Serial.print(" SILVER ");
      return Silver;
    }else{
      Serial.print("NONE");
      return White;
    }
    Serial.println();
}

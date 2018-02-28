#include "ColorSensor.h"

ColorSensor::ColorSensor(ShiftRegister74HC595& sr) : sr(sr){
  sr.set(S0, HIGH); 
  sr.set(S1, LOW);
  pinMode(sensorOut, INPUT);
}
bool ColorSensor::withinRange(int input, int value){
  if(input + input * range > value && input - input*range < value)    
    return true;
  return false;
}

Color ColorSensor::getColor(){ 
  
  sr.set(S2,LOW);
  sr.set(S3,LOW);
  redFrequency = abs(map(pulseIn(sensorOut, LOW), 152, 27, 0, 255));
  //redFrequency = pulseIn(sensorOut, LOW);

 
  sr.set(S2,LOW);
  sr.set(S3,HIGH);
  blueFrequency = abs(map(pulseIn(sensorOut, LOW), 142, 23, 0, 255));
  //blueFrequency = pulseIn(sensorOut, LOW);

  
  sr.set(S2,HIGH);
  sr.set(S3,HIGH);
  greenFrequency = abs(map(pulseIn(sensorOut, LOW), 171, 23, 0, 255));
  //greenFrequency = pulseIn(sensorOut, LOW);
//
  if(calibrar){
    Serial.print("R= ");
    Serial.print(redFrequency);
    Serial.print(" G= ");
    Serial.print(greenFrequency);
    Serial.print(" B= ");
    Serial.println(blueFrequency);
  }

  if((withinRange(redFrequency, 255)&& withinRange(greenFrequency, 242))&& withinRange(blueFrequency, 252)){
    if(calibrar)
      Serial.print(" WHITE ");
    return Color::White;
  }else if(((withinRange(redFrequency, 170)&& withinRange(greenFrequency, 160))&& withinRange(blueFrequency, 170)) ||((withinRange(redFrequency, 10)&& withinRange(greenFrequency, 10))&& withinRange(blueFrequency, 10)) ){
    if(calibrar)
      Serial.print(" BLACK ");
    return Color::Black;
  }else if((withinRange(redFrequency, 283)&& withinRange(greenFrequency, 156))&& withinRange(blueFrequency, 233)){
    if(calibrar)
      Serial.print(" PINK ");
    return Color::Pink;
  }else if((withinRange(redFrequency, 285)&& withinRange(greenFrequency, 215))&& withinRange(blueFrequency, 170)){
    if(calibrar)
      Serial.print(" ORANGE ");
    return Color::Orange;
  }else if((withinRange(redFrequency, 261)&& withinRange(greenFrequency, 253))&& withinRange(blueFrequency, 190)){
    if(calibrar)
      Serial.print(" YELLOW ");
    return Color::Yellow;
  }else if(redFrequency > blueFrequency){
    if(calibrar)
      Serial.print(" RED ");
    return Color::Red;
  }else if(blueFrequency > redFrequency){
    if(calibrar)
      Serial.print(" BLUE");
    return Color::Blue;
  }else{
      return Color::None;
  }
}

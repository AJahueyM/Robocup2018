#include "VisionSensor.h"

VisionSensor::VisionSensor(){
    pinMode(sLetterPin, INPUT);
    pinMode(hLetterPin, INPUT);
    pinMode(uLetterPin, INPUT);
    pinMode(directionLetterPin, INPUT);
}

VisionResponse VisionSensor::getStatus(){
    VisionResponse response;
    if(digitalRead(hLetterPin){
        response.victim = Harmed;
    }else if(digitalRead(sLetterPin)){
        response.victim = Stable;
    }else if(digitalRead(Unharmed)){
        response.victim = Unharmed;
    }else{
        response.victim = None;
    }

    if(digitalRead(directionLetterPin)){
        response.dir = Right;
    }else{
        response.dir = Left;
    }
    return response;
}
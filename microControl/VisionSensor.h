#ifndef __VISIONSENSOR_H_
#define __VISIONSENSOR_H_
#include "Utils.h"
#include <Arduino.h>

enum VisionVictim {
    None,
    Harmed,
    Stable,
    Unharmed
};

struct VisionResponse {
    VisionVictim victim;
    Direction dir;
};

class VisionSensor{
public:
    static VisionSensor& getInstance(){
        static VisionSensor vision;
        return vision;
    };
    VisionResponse getStatus();
private:
    VisionSensor();
    VisionSensor &operator=(const VisionSensor &);
    uint8_t sLetterPin = 10, hLetterPin = 11, uLetterPin = 12, directionLetterPin = 13;

};
#endif
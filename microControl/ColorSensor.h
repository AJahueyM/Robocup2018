#ifndef __COLORSENSOR_H_
#define __COLORSENSOR_H_
#include "Arduino.h"
#include "Utils.h"

class ColorSensor{
private:
  uint8_t redFrequency = 0, blueFrequency = 0 , greenFrequency = 0;
  double range = .4, timeoutReads = 60;
  long lastReadTime = 0;
  const uint8_t S0 = 33;
  const uint8_t S1 =  30;
  const uint8_t  S2 = 27;
  const uint8_t  S3 = 29;
  const uint8_t sensorOut =  25;
  bool calibrar = true;
  bool withinRange(int input, int value);

public:
  ColorSensor();
  Color getColor();

};
#endif

#ifndef __COLORSENSOR_H_
#define __COLORSENSOR_H_
#include "Arduino.h"
#include "Utils.h"


class ColorSensor{
private:
  int redFrequency = 0, blueFrequency = 0 , greenFrequency = 0;
  double range = .4, timeoutReads = 60;
  long lastReadTime = 0;
  const int S0 = 33;
  const int S1 =  30;
  const int  S2 = 27;
  const int  S3 = 29;
  const int sensorOut =  25;
  bool calibrar = true;
  bool withinRange(int input, int value);

public:
  ColorSensor();
  Color getColor();

};

#endif

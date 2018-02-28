#ifndef __COLORSENSOR_H_
#define __COLORSENSOR_H_
#include "Arduino.h"
#include <ShiftRegister74HC595.h>

enum Color {
  Pink,
  Yellow,
  Orange,
  Red, 
  Blue,
  White,
  Black,
  Green,
  None
};
class ColorSensor{
private:
  ShiftRegister74HC595& sr;
  int redFrequency = 0, blueFrequency = 0 , greenFrequency = 0;
  double range = .2;
  const int S0 = 2;
  const int S1 =  1;
  const int  S2 = 3;
  const int  S3 = 4;
  const int sensorOut =  3;
  bool calibrar = false;
  bool withinRange(int input, int value);

public:
  ColorSensor(ShiftRegister74HC595& sr);
  Color getColor();

};

#endif
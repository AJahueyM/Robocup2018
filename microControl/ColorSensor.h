#ifndef __COLORSENSOR_H_
#define __COLORSENSOR_H_
#include "Arduino.h"
#include "Utils.h"
#include "LCD.h"

struct ColorProfile{
  short int r;
  short int g;
  short int b;
};

class ColorSensor{
private:
  uint8_t redFrequency = 0, blueFrequency = 0 , greenFrequency = 0;
  double range = .4, timeoutReads = 60;
  long lastReadTime = 0;
  const uint8_t S0 = 25;
  const uint8_t S1 =  23;
  const uint8_t  S2 = 24;
  const uint8_t  S3 = 26;
  const uint8_t sensorOut =  22;
  const uint8_t sensorReadings = 20;
  static ColorProfile whiteProfile, blackProfile, silverProfile;
  bool calibrar = true;
  bool withinRange(int input, int value);
  ColorSensor();
  ColorSensor &operator=(const ColorSensor &);
  LCD lcd = LCD::getInstance();

  short int getRedFrequency();
  short int getGreenFrequency();
  short int getBlueFrequency();

public:
  static ColorSensor& getInstance(){
    static ColorSensor instance;
    return instance;
  }
  Color getColor();
  void calibrateWhite();
  void calibrateBlack();
  void calibrateSilver();

};
#endif

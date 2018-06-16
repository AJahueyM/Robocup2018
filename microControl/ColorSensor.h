#ifndef __COLORSENSOR_H_
#define __COLORSENSOR_H_
#include "Arduino.h"
#include "Utils.h"
#include "LCD.h"
#include <EEPROM.h>

struct ColorProfile{
  uint8_t r;
  uint8_t g;
  uint8_t b;
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

  uint8_t getRedFrequency();
  uint8_t getGreenFrequency();
  uint8_t getBlueFrequency();

  uint8_t whiteDirR = 0;
  uint8_t whiteDirG = 1;
  uint8_t whiteDirB = 2;
 
  uint8_t blackDirR = 3;
  uint8_t blackDirG = 4;
  uint8_t blackDirB = 5;

  uint8_t silverDirR = 6;
  uint8_t silverDirG = 7;
  uint8_t silverDirB = 8;


public:
  static ColorSensor& getInstance(){
    static ColorSensor instance;
    return instance;
  }
  Color getColor();
  void calibrateWhite();
  void calibrateBlack();
  void calibrateSilver();
  void saveProfiles();

};
#endif

#ifndef __LED_H_
#define __LED_H_
#include "Arduino.h"
#include "Utils.h"
#include <ArduinoSTL.h>
using namespace std;

class Led{
public:
  explicit Led(uint8_t pin);
  explicit Led(vector<uint8_t> pin);
  void blink(unsigned int times, bool fadeInOut = false, unsigned int periodMs = 100);
  void setState(bool state);
  bool getState() const;
  bool toggle();
  void setMaxIntensity(uint8_t intensity);
  void setMinIntensity(uint8_t intensity);
  void addPin(uint8_t pin);
private:
  void setupPins();
  void setPins(uint8_t value);
  maxIntensity = 255, minIntensity = 0;
  vector<uint8_t> pins;
  bool state;

};

#endif

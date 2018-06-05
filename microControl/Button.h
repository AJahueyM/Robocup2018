#ifndef __BUTTON_H_
#define __BUTTON_H_
#include "Arduino.h"

class Button {
public:
	Button(uint8_t digitalPin);
	bool getState();
private:
	uint8_t digitalPin;
};
#endif
#ifndef __BUTTON_H_
#define __BUTTON_H_
#include "Arduino.h"

class Button {
public:
	Button(int digitalPin);
	bool getState();
private:
	int digitalPin;
};
#endif
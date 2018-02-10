#ifndef __BUTTON_HEADER_
#define __BUTTON_HEADER_
#include "Arduino.h"

class Button {
public:
	Button(int digitalPin);
	bool getState();
private:
	int digitalPin;
};


#endif
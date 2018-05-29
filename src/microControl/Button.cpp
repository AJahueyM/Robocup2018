#include "Button.h"

Button::Button(uint8_t digitalPin){
	this->digitalPin = digitalPin;
}

bool Button::getState(){
	return digitalRead(digitalPin);
}
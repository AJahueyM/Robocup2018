#include "Button.h"

Button::Button(int digitalPin){
	this->digitalPin = digitalPin;
}

bool Button::getState(){
	return digitalRead(digitalPin);
}
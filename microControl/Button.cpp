#include "Button.h"

Button::Button(uint8_t digitalPin){
	this->digitalPin = digitalPin;
	pinMode(digitalPin, INPUT);
}

Button::Button(int digitalPin){
	this->digitalPin = digitalPin;
	pinMode(digitalPin, INPUT);
}

bool Button::getState(){
	return digitalRead(digitalPin);
}
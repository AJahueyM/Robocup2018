#include "Led.h"

Led::Led(uint8_t pin){
	pins.emplace_back(pin);
	setupPins();
}

Led::Led(vector<uint8_t> pins){
	this->pins = pins;
	setupPins();
}

void Led::blink(unsigned int times, bool fadeInOut, unsigned int periodMs){
	if(fadeInOut){
		int duration = periodMs * times
		int multiplier = maxIntensity - minIntensity;
		if(multiplier<=0){
			multiplier = 255
			minIntensity = 0
		}
		long millisStart = millis();
		long elapsed = 0;
		while(elapsed <= duration){
			elapsed = millis() - millisStart;
			uint8_t output = (multiplier/2)*sin( (2*PI / periodMs) * elapsed - PI/2) + (multiplier/2 + minIntensity);
			setPins(output);
		}
	}else{
		for(unsigned int i = 0, i < times, ++i){
			setPins(minIntensity);
			delay(periodMs / 2);
			setPins(maxIntensity);
			delay(periodMs / 2);
		}
	}
}

void Led::setState(bool state){
	this->state = state;

	if(state){
		setPins(maxIntensity);
	}else{
		setPins(minIntensity);
	}
}

bool Led::getState() const{
	return state;
}

bool Led::toggle(){
	state = !state;

	if(state){
		setPins(maxIntensity);
	}else{
		setPins(minIntensity);
	}
	return state;
}

void Led::setMaxIntensity(uint8_t intensity){
	maxIntensity = intensity;
}

void Led::setMinIntensity(uint8_t intensity){
	minIntensity = intensity;
}

void Led::addPin(uint8_t pin){
	pins.emplace_back(pin);
}

void Led::setupPins(){
	for(uint8_t pin : pins){
		pinMode(pin, OUTPUT);
		analogWrite(pin, minIntensity);
	}	
}

void Led::setPins(uint8_t value){
	for(uint8_t pin : pins){
		analogWrite(pin, value);
	}	
}

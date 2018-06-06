#include "Sharp.h"

Sharp::Sharp(uint8_t analogPort){
	this->analogPort = analogPort;
	filter.setSource(&distance);
}

uint8_t Sharp::getDistance(){
	for(int i = 0; i < readForFilter; ++i){
		uint16_t value = analogRead(analogPort);
		if (value < 16)  value = 16;
	    distance =  2076.0 / (value - 11.0);
		distance = filter.kalmanFilter();
	}

	return distance;
}

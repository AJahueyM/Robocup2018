#include "Sharp.h"

Sharp::Sharp(uint8_t analogPort) : SharpIR(analogPort, 430){
	this->analogPort = analogPort;
	filter.setSource(&distance);
}

float Sharp::getDistance(){

	return SharpIR.distance();  
}

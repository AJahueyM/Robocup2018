#include "Sharp.h"

Sharp::Sharp(uint8_t analogPort){
	this->analogPort = analogPort;
	filter.setSource(&distance);
}

uint8_t Sharp::getDistance(){
	for(int i = 0; i < readForFilter; ++i){
		double volts = analogRead(analogPort)  * 0.0048828125;

		distance = 13 * pow(volts, -1);
		if(distance > 30)
			distance = 30;
		if(distance < 6)
			distance = 30;
		//average += distance;
		distance = filter.kalmanFilter();
	}
	
	return distance;
}

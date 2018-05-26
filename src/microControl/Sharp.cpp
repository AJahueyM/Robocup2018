#include "Sharp.h"

Sharp::Sharp(int analogPort){
	this->analogPort = analogPort;
	filter.setSource(&distance);
}

int Sharp::getDistance(){
	int average = 0;
	for(int i = 0; i < readForFilter; ++i){
		double volts = analogRead(analogPort)  * 0.0048828125;

		distance = 13 * pow(volts, -1);
		if(distance > 30)
			distance = 30;
		if(distance < 4)
			distance = 4;
		//average += distance;
		distance = filter.kalmanFilter();
	}
	
	return distance;
}

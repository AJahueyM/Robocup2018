#include "Sharp.h"

Sharp::Sharp(int analogPort){
	this->analogPort = analogPort;
	filter.setSource(&distance);
}

int Sharp::getDistance(){
	double volts = analogRead(analogPort)  * 0.0048828125;

	distance = 13 * pow(volts, -1);
	//distance = filter.kalmanFilter();
	if(distance > 30)
		distance = 30;
	if(distance < 4)
		distance = 4;
	return distance;
}
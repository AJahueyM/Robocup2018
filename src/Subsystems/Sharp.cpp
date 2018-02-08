#include "Sharp.h"

Sharp::Sharp(int analogPort){
	this->analogPort = analogPort;
}

int Sharp::getDistance(){
	double volts = analogRead(analogPort)  *0.0048828125;
	int distance = 13 * pow(volts, -1);
	return distance;
}
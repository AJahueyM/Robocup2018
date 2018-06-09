#include "Sharp.h"

Sharp::Sharp(uint8_t analogPort){
	this->analogPort = analogPort;
	filter.setSource(&distance);
}

uint8_t Sharp::getDistance(){
	for(int i = 0; i < readForFilter; ++i){
		float volts = analogRead(analogPort)*0.0048828125;  // value from sensor * (5/1024)
		int distance = 13*pow(volts, -1); // worked out from datasheet graph

		if (distance <= 30){
			return distance;   // print the distance
		}else{
			return 31;
		}
		distance = filter.kalmanFilter();
	}

	return distance;
}

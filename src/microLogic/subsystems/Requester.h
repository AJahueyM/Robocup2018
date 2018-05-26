#ifndef __REQUESTER_HEADER_
#define __REQUESTER_HEADER_
#include "Comms.h"
#include <Arduino.h>
enum Sensor{
	DistanceRight,
	DistanceLeft,
	DistanceFront,
	HeatRight,
	HeatLeft,
	Yaw,
	Pitch,
	Color
};

class Requester {
public:
	static Requester &getInstance() {
		static Requester singletonInstance;
		return singletonInstance;
	}
	int requestSensorValue(Sensor sensor);
private:
	Requester();
	Requester &operator=(const Requester &);
	Sensor currentSensor; /// The sensor that is currently being polled	
	Comms& comms = Comms::getInstance();
	bool megaAknowledged = false;
	long requestTimeout = 1000, lastTime = millis();

};

#endif
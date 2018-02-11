#ifndef __REQUESTER_HEADER_
#define __REQUESTER_HEADER_
#include "Comms.h"
enum Sensor{
	DistanceRight,
	DistanceLeft
	///TODO= DEFINE ALL OF THIS
};

class Requester {
public:
	Requester(Comms& comms);
	double requestSensorValue(Sensor sensor);
private:
	Comms& comms;

};

#endif
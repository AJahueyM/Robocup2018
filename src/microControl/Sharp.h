
#ifndef __SHARP_HEADER_
#define __SHARD_HEADER_
#include <Arduino.h>
#include "Filter.h"

class Sharp {
public:
	Sharp(int analogIn);
	int getDistance();
private:
	Filter filter = Filter(1,3,10);
	float distance;
	int analogPort;
};


#endif
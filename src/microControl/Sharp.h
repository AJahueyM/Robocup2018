#ifndef __SHARP_H_
#define __SHARD_H_
#include <Arduino.h>
#include "Filter.h"

class Sharp {
public:
	Sharp(int analogIn);
	int getDistance();
private:
	Filter filter = Filter(10,3,10);
	float distance;
	int readForFilter = 25;
	int analogPort;
};
#endif

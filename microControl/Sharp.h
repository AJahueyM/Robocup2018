#ifndef __SHARP_H_
#define __SHARP_H_
#include <Arduino.h>
#include "Filter.h"

class Sharp {
public:
	Sharp(uint8_t analogIn);
	uint8_t getDistance();
private:
	Filter filter = Filter(10,3,10);
	float distance;
	int readForFilter = 25;
	uint8_t analogPort;
};
#endif

#ifndef __SHARP_H_
#define __SHARP_H_
#include <Arduino.h>
#include "Filter.h"
#include <SharpIR.h>

class Sharp {
public:
	Sharp(uint8_t analogIn);
	float getDistance();
private:
	SharpIR SharpIR;
	Filter filter = Filter(10,3,10);
	float distance, average;
	int readForFilter = 25;
	uint8_t analogPort;
};
#endif

#ifndef __SHARP_HEADER_
#define __SHARD_HEADER_
#include <Arduino.h>

class Sharp {
public:
	Sharp(int analogIn);
	int getDistance();
private:
	int analogPort;
};


#endif
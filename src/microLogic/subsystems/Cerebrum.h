#ifndef __CEREBRUM_HEADER_
#define __CEREBRUM_HEADER_
#include "Executer.h"
#include "Requester.h"
#include "Map.h"
#include "Path.h"
#include "Arduino.h"

class Cerebrum {
public:
	Cerebrum();
	void updateMap();
	void makeDecision();
	void executeDecision();
private:
	void updateCurrentTile();
	void extendMap();
};

#endif
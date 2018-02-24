#ifndef __CEREBRUM_HEADER_
#define __CEREBRUM_HEADER_
#include "Executer.h"
#include "Requester.h"
#include "Map.h"
#include "Path.h"
#include "Arduino.h"

class Cerebrum {
public:
	static Cerebrum &getInstance() {
		static Cerebrum singletonInstance;
		return singletonInstance;
	}
	void updateMap();
	void makeDecision();
	void executeDecision();
private:
	Cerebrum();
	Map* currentMap;
	Cerebrum &operator=(const Cerebrum &);	
	Tile getCurrentTile();
	void updateCurrentTile();
	void extendMap();
};

#endif
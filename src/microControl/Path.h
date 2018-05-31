#ifndef __PATH_H_
#define __PATH_H_
#include "Coord.h"
#include <ArduinoSTL.h>
using namespace std;

class Path {
public:
	Path();
	Path(vector<Coord> path);
	Coord getCoordAt(uint8_t index);
	uint8_t getLength();
	vector<Coord> getPath();
private:
	vector<Coord> path;
};
#endif
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
	uint8_t getCost();
	uint8_t setCost(uint8_t cost);
	vector<Coord> getPath();
	void print();
private:
	vector<Coord> path;
	uint8_t cost = 0;
};
#endif
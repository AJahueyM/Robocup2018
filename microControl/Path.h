#ifndef __PATH_H_
#define __PATH_H_
#include "Coord.h"
#include "Absis.h"

class Path {
public:
	Path();
	Path(Absis<Coord> path);
	Coord getCoordAt(uint8_t index);
	uint8_t getLength();
	uint8_t getCost();
	uint8_t setCost(uint8_t cost);
	void setValid(bool valid);
	bool getValid();
	Absis<Coord> getPath();
	void print();
private:
	Absis<Coord> path;
	uint8_t cost = 254;
	bool valid = false;
};
#endif
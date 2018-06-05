#ifndef __PATH_H_
#define __PATH_H_
#include "Coord.h"
#ifdef ARDUINO
#include <ArduinoSTL.h>
#else
#include <vector>
#include <iostream>
#endif

using namespace std;

class Path {
public:
	Path();
	Path(vector<Coord> path);
	Coord getCoordAt(uint8_t index);
	uint8_t getLength();
	uint8_t getCost();
	uint8_t setCost(uint8_t cost);
	void setValid(bool valid);
	bool getValid();
	vector<Coord> getPath();
	void print();
private:
	vector<Coord> path;
	uint8_t cost = 254;
	bool valid = false;
};
#endif
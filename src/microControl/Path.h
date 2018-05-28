#ifndef __PATH_H_
#define __PATH_H_
#include "Coord.h"
#include <vector>
using namespace std;

class Path {
public:
	Path();
	Path(vector<Coord> path);
	Coord getCoordAt(int index);
	int getLenght();
	vector<Coord> getPath();
private:
	vector<Coord> path;
};
#endif
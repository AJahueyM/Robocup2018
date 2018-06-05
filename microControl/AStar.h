#ifndef __ASTART_H_
#define __ASTART_H_
#include "Path.h"
#include "Tile.h"
#include "Utils.h"
#include "Absis.h"
#include "Coord.h"

#ifdef ARDUINO
#include <ArduinoSTL.h>
#else
#include <vector>
#include <math.h>
#include <iostream>
#endif

class AStar{
public:
    static Path getPath(Coord start, Coord end, Absis<Absis<Tile>>& maze);
private:
    static int heuristic(Coord start, Coord end);
};
#endif
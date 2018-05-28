#ifndef __MAP_H_
#define __MAP_H_
#include "Tile.h"
#include "Coord.h"
#include "Path.h"
#include <ArduinoSTL.h>
using namespace std;

class Map{
public:
	Map(Tile initialTile);
	Tile& getTileAt(Coord coord);
	void setTileAt(Coord coord, Tile newTile);
	Coord getRobotCoord();
	void setRobotCoord(Coord coord);
	void expandMap();
	Path getPathTo(Coord targetCoord);
	uint8_t getWidth();
	uint8_t getHeight();
	int getNonVisitedTiles();
private:
	vector<vector<Tile>> tileMap;
	void updateWalls();
	void updateCoords();
	void checkPockets();
	Coord robotCoord;
	byte mockIdentity = B11111101;
	Tile mockTile;
	Direction robotDirection;

};
#endif


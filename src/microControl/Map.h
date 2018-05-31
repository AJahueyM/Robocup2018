#ifndef __MAP_H_
#define __MAP_H_
#include "Tile.h"
#include "Coord.h"
#include "Path.h"
#include "Absis.h"
#include <ArduinoSTL.h>
using namespace std;

class Map{
public:
	Map(Absis<Absis<Tile>>& tileMap);
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
	Absis<Absis<Tile>>& getTileMap();
	void setTileMap(Absis<Absis<Tile>> tileMap); 
private:
	Absis<Absis<Tile>> tileMap;
	void updateWalls();
	void updateCoords();
	void checkPockets();
	void updateNeighbors();
	void updateMap();
	Coord robotCoord;
	byte mockIdentity = B11111101;
	Tile mockTile = Tile(mockIdentity, mockIdentity);
	Direction robotDirection;

};
#endif


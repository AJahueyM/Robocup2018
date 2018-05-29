#ifndef __MAP_H_
#define __MAP_H_
#include "Tile.h"
#include "Coord.h"
#include "Path.h"
#include <ArduinoSTL.h>
using namespace std;

class Map{
public:
	Map(vector<vector<Tile>>& tileMap);
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
	vector<vector<Tile>>& getTileMap();
	void setTileMap(vector<vector<Tile>> tileMap);
private:
	vector<vector<Tile>> tileMap;
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


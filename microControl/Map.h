#ifndef __MAP_H_
#define __MAP_H_
#include "Tile.h"
#include "Coord.h"
#include "Path.h"
#include "Absis.h"
#include "LCD.h"
#include "Ramp.h"
#ifdef ARDUINO
#include <ArduinoSTL.h>
#else
#include <iostream>
#endif
using namespace std;

class Map{
public:
	Map(Absis<Absis<Tile>>& tileMap, const uint8_t levelNum);
	Map(Tile initialTile, const uint8_t levelNum);
	uint8_t getLevelNum();
	Tile& getTileAt(Coord coord);
	void setTileAt(Coord coord, Tile newTile);
	Coord getRobotCoord();
	void setRobotCoord(Coord coord);
	void expandMap();
	Path getPathTo(Coord targetCoord);
	uint8_t getWidth();
	uint8_t getHeight();
	int getNonVisitedTiles();
	vector<Coord> getCandidates();
	Absis<Absis<Tile>>& getTileMap();
	void setTileMap(Absis<Absis<Tile>> tileMap); 
	void addRamp(Ramp ramp);
	Ramp* getRampAt(Coord coord);
	vector<Coord> getRampsCoords();
	Absis<Ramp>& getRamps();
	bool usedAllRamps();
	bool wasCompleted();
	static void createRamp(Map* startMap, Tile* start, Map* endMap, Tile* end);
	LCD& lcd = LCD::getInstance();
private:
	Absis<Absis<Tile>> tileMap;
	Absis<Ramp> ramps;
	void updateWalls();
	void updateCoords();
	void checkPockets();
	void updateNeighbors();
	void updateMap();
	Coord robotCoord;
	byte mockIdentity = B11111101;
	Tile mockTile = Tile(mockIdentity, mockIdentity);
	Direction robotDirection;
	uint8_t levelNum; 
};
#endif


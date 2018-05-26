#ifndef __MAP_HEADER_
#define __MAP_HEADER_
#include <ArduinoSTL.h>
#include "Tile.h"
#include "Coord.h"
#include "Path.h"

using namespace std;


class Map{
public:
	Map(Tile initialTile);
	void addColumn(Direction side);
	void addRow(Direction side);
	Tile& getTileAt(Coord coord);
	void setTileAt(Coord coord, Tile newTile);
	Coord getRobotCoord();
	void setRobotCoord(Coord coord);
	Path getPathTo(Coord targetCoord);
private:
	vector<vector<Tile>> tileMap;
	uint8_t height, width;
	Coord robotCoord;
	Direction robotDirection;

};
#endif


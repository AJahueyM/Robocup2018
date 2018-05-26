#ifndef __TILE_HEADER_
#define __TILE_HEADER_
#include "TileWall.h"
#include "TileBump.h"

enum TileColor {
	White,
	Black,
	Silver
};

enum Direction {
	Up,
	Down,
	Left,
	Right
};

struct TileConfig {
	TileWall up, down, left, right;
	TileBump bump;
	TileColor color = White;
	bool isRamp;
};

class Tile {
public:
	Tile(TileConfig config);
	bool isRamp();
	bool wallExists(Direction side);
	bool heatVictimExists(Direction side);
	bool visionVictimExists(Direction side);
	TileColor getColor();
	double getBumpWeight();
private:
	TileWall up, down, left, right;
	TileBump bump;
	TileColor color;
	bool isRamp;
};

#endif

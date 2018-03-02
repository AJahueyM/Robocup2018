#ifndef __TILE_HEADER_
#define __TILE_HEADER_
#include "TileWall.h"
#include "TileBump.h"
#include "Arduino.h"
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
	static const byte wallUpMask = B10000000;
	static const byte wallRightMask = B01000000;
	static const byte wallDownMask = B00100000;
	static const byte wallLeftMask = B00010000;
	static const byte isRampMask = B00001000;
	static const byte hasVictimMask = B00000100;
	static const byte isBlackMask = B00000010;
	static const byte isCheckpointMask = B00000001;

	Tile(TileConfig config);
	bool isRamp();
	bool wallExists(Direction side);
	bool hasVictim();
	TileColor getColor();
	double getBumpWeight();
private:
	TileWall up, down, left, right;
	TileBump bump;
	TileColor color;
	bool isTileRamp;
	byte identity = B00000000;
};

#endif



/*
	00000000
	1: wall up
	2: wall right
	3: wall down
	4: wall left
	5: ramp
	6: has victim 
	7: isBlack
	8: isCheckpoint
*/

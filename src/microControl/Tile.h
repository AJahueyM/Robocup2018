#ifndef __TILE_HEADER_
#define __TILE_HEADER_
#include "Arduino.h"
#include "Utils.h"


class Tile {
public:
	static const byte wallUpMask = B10000000;
	static const byte wallRightMask = B01000000;
	static const byte wallDownMask = B00100000;
	static const byte wallLeftMask = B00010000;
	static const byte isRampMask = B00001000;
	static const byte leftKitMask = B00000100;
	static const byte isBlackMask = B00000010;
	static const byte isCheckpointMask = B00000001;
	static const byte maskVisited = B10000000;
	static const byte hasBumpMask = B01000000;
	Tile();
	Tile(byte identity);
	Tile(byte identity, byte identity2);
	bool isRamp();
	bool wallExists(Direction side);
	bool setWall(Direction side, bool value);
	void setLeftKit(bool value);
	bool getLeftKit();
	bool wasVisited();
	bool hasBump();
	void visited(bool value);
	Color getColor();
private:
	byte identity = 0;
	byte identity2 = 0;
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

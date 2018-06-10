#ifndef __TILE_H_
#define __TILE_H_
#include "Utils.h"
#include "TileMasks.h"
#include "Node.h"

class Tile : public Node{
public:
	Tile();
	explicit Tile(int x, int y);
	explicit Tile(byte identity);
	explicit Tile(byte identity, byte identity2);
	bool isRamp();
	void setRamp(bool isRamp);
	bool wallExists(Direction side) override;
	void setWall(Direction side, bool value) override;
	void setLeftKit(bool value);
	bool getLeftKit();
	void setColor(Color color);
	bool wasVisited();
	bool hasBump();
	void visited(bool value);
	byte getIdentity1();
	byte getIdentity2();
	Color getColor();
	BumpLevel getBumpLevel() ;
	void setBumpLevel(BumpLevel bumpType);
	uint8_t getCost() override;

	Tile& operator=(const Tile& a){
		if(this != &a){
			this->identity = a.getIdentity1();
			this->identity2 = a.getIdentity2();
		}
		return *this;
	}
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

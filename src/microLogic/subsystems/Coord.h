#ifndef __COORD_HEADER_
#define __COORD_HEADER_

class Coord {
public:
	Coord(uint8_t x, uint8_t y);
	uint8_t getX();
	uint8_t getY();
private:
	uint8_t x, y;
};

#endif
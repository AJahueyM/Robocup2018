#ifndef __COORD_HEADER_
#define __COORD_HEADER_

class Coord {
public:
  Coord(){
    this->x = 0;
    this->y = 0;
  }
  
	Coord(uint8_t x, uint8_t y){
	  this->x = x;
    this->y = y;
	}
	uint8_t getX();
	uint8_t getY();
private:
	uint8_t x, y;
};

#endif

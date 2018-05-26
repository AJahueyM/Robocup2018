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
	uint8_t getX(){
		return this->x;
	}
	uint8_t getY(){
		return this->y;
	}
	void setX(uint8_t x){
		this->x = x;
	}
	void setY(uint8_t y){
		this->y = y;
	}
private:
	uint8_t x, y;
};

#endif

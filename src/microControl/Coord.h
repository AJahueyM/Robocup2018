#ifndef __COORD_H_
#define __COORD_H_
#include <Arduino.h>

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
	uint8_t getX() const{
		return this->x;
	}
	uint8_t getY() const{
		return this->y;
	}
	void setX(uint8_t x){
		this->x = x;
	}
	void setY(uint8_t y){
		this->y = y;
	}
	bool operator==(const Coord& c){
		return this->x == c.getX() && this->y == c.getY();
	}
private:
	uint8_t x, y;
};
#endif

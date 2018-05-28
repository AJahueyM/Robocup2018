#ifndef __COORD_H_
#define __COORD_H_

class Coord {
public:
	Coord(){
		this->x = 0;
		this->y = 0;
	}
  
	Coord(int x, int y){
		this->x = x;
		this->y = y;
	}
	int getX() const{
		return this->x;
	}
	int getY() const{
		return this->y;
	}
	void setX(int x){
		this->x = x;
	}
	void setY(int y){
		this->y = y;
	}
	bool operator==(const Coord& c){
		return this->x == c.getX() && this->y == c.getY();
	}
private:
	int x, y;
};
#endif

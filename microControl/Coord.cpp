#include "Coord.h"

Coord::Coord(){
    this->x = 0;
    this->y = 0;
}

Coord::Coord(uint8_t x, uint8_t y){
    this->x = x;
    this->y = y;
}
int Coord::getX() const{
    return (int) this->x;
}
int Coord::getY() const{
    return (int) this->y;
}
void Coord::setX(uint8_t x){
    this->x = x;
}
void Coord::setY(uint8_t y){
    this->y = y;
}
bool Coord::operator==(const Coord& c){
    return this->x == c.getX() && this->y == c.getY();
}
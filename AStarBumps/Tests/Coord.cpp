#include "Coord.h"

Coord::Coord(){
    this->x = 0;
    this->y = 0;
}

Coord::Coord(uint8_t x, uint8_t y){
    this->x = x;
    this->y = y;
}
uint8_t Coord::getX() const{
    return this->x;
}
uint8_t Coord::getY() const{
    return this->y;
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
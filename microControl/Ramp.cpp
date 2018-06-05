#include "Ramp.h"

Ramp::Ramp(){

}

Ramp::Ramp(Tile* origin, uint8_t targetFloor){
    this->origin = origin;
    this->targetFloor = targetFloor;
}

Coord Ramp::getOrigin() const{
    return *this->origin;
}

Coord Ramp::getEnd() const{
    return *this->end;
}

void Ramp::setEnd(Tile* end){
    this->end = end;
}

uint8_t Ramp::getTargetFloor() const{
    return targetFloor;
}

void Ramp::setUsed(bool used){
    this->used = used;
}

bool Ramp::getUsed(){
    return used;
}

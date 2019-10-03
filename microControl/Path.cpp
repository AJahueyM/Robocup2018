#include "Path.h"

Path::Path(){

}

Path::Path(Absis<Coord> path){
    this->path = path;
}

Coord Path::getCoordAt(uint8_t index){
    if(index >= 0 && index < path.size()){
        return path[index];
    }
}

uint8_t Path::getLength(){
    return path.size();
}

uint8_t Path::getCost(){
    return cost;
}

uint8_t Path::setCost(uint8_t cost){
    this->cost  = cost;
}

void Path::setValid(bool valid){
    this->valid = valid;
}

bool Path::getValid(){
    return valid;
}

Absis<Coord> Path::getPath(){
    return path;
}

void Path::print(){
    for(int i = 0; i < path.size(); ++i){
        Coord coord = path[i];
    }
}

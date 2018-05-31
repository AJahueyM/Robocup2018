#include "Path.h"

Path::Path(){

}

Path::Path(vector<Coord> path){
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

vector<Coord> Path::getPath(){
    return path;
}
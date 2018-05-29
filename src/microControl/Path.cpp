#include "Path.h"

Path::Path(){

}

Path::Path(vector<Coord> path){
    this->path = path;
}

Coord Path::getCoordAt(int index){
    if(index >= 0 && index < path.size()){
        return path[index];
    }
}

int Path::getLength(){
    return path.size();
}

vector<Coord> Path::getPath(){
    return path;
}
#include "Tile.h"

Tile::Tile(TileConfig tile){

}

bool Tile::isRamp(){
    byte result = identity & isRampMask;
    return result != 0;
}

bool Tile::wallExists(Direction side){
    byte result;
    switch(side){
       	case Up:
           result = identity & wallUpMask;
           break;
        case Down:
            result = identity & wallDownMask;
            break;
        case Left:
            result = identity & wallLeftMask;
            break;

        case Right:
            result = identity & wallRightMask;
        break;
    }
    return result != 0;
}

bool Tile::heatVictimExists(Direction side){

}

bool Tile::visionVictimExists(Direction side){

}

TileColor Tile::getColor(){
    if(identity & isBlackMask != 0){
        return Black;
    }
    if(identity & isCheckpointMask != 0){
        return Silver;
    }
    else
    return White;
}

double Tile::getBumpWeight(){
	
}
#include "Tile.h"

Tile::Tile(){
    identity = 0;
    identity2 = 0;
}

Tile::Tile( int x, int y) : Node(x, y){
    
}

Tile::Tile(byte identity){
    this->identity = identity;
}

Tile::Tile(byte identity, byte identity2){
    this->identity = identity;
    this->identity2 = identity2;
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

void Tile::setWall(Direction side, bool value){
    switch(side){
        case Up:
            if(value)
                identity = identity | wallUpMask;
            else 
                identity = identity & ~wallUpMask;
            break;
           
        case Down:
            if(value)
                identity = identity | wallDownMask;
            else 
                identity = identity & ~wallDownMask;
            break;

        case Left:
            if(value)
                identity = identity | wallLeftMask;
            else 
                identity = identity & ~wallLeftMask;
            break;

        case Right:
            if(value)
                identity = identity | wallRightMask;
            else
                identity = identity & ~wallRightMask;
            break;
    }
}

void Tile::setLeftKit(bool value){
    if(value)
        identity = identity | leftKitMask;
    else
        identity = identity & ~leftKitMask;
}

bool Tile::getLeftKit(){
    byte result = identity & leftKitMask;
    return result != 0;
}

bool Tile::wasVisited() {
    byte result = identity2  & maskVisited;
    return result != 0;
}

void Tile::visited(bool value){
    if(value){
        identity2 = identity2 | maskVisited;
    }else{
        identity2 = identity2 & ~maskVisited;
    }
}

Color Tile::getColor(){
    byte result = B00000000;

    result = identity & isBlackMask;
    if(result != 0){
        return Black;
    }
    result = identity & isCheckpointMask;

    if(result != 0){
        return Silver;
    }
    return White;
}

BumpLevel Tile::getBumpLevel(){
    byte result = B00000000;

    result = identity2 & smallBumpMask;

    if(result != 0){
        return Small;
    }

    result = identity2 & mediumBumpMask;

    if(result != 0){
        return Medium;
    }

    result = identity2 & maxBumpMask;

    if(result != 0){
        return Max;
    }

    return None;

}

void Tile::setBumpLevel(BumpLevel bumpType){
    byte result;
    switch(bumpType){
        case Small:
            result = identity2 | smallBumpMask;
            break;
        case Medium:
            result = identity2 | mediumBumpMask;
            break;
        case Max:
            result = identity2 | maxBumpMask;
            break;
    }

    identity2 = result;

}

uint8_t Tile::getCost(){
    if(this->getBumpLevel() == Small){
        return 5;
    }

    if(this->getBumpLevel() == Medium){
        return 10;
    }

    if(this->getBumpLevel() == Max){
        return 100;
    }

    return 1;
}
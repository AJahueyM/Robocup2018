#ifndef __RAMP_H_
#define __RAMP_H_
#include "Tile.h"

class Ramp {
public:
    Ramp();
    Ramp(Tile* origin, uint8_t targetFloor);
    Coord getOrigin() const;
    Coord getEnd() const;
    void setEnd(Tile* end);
    uint8_t getTargetFloor() const;
    void setUsed(bool used);
    bool getUsed();

private:
    Tile* origin;
    Tile* end;
    uint8_t targetFloor = 0;
    bool used = false;
};
#endif
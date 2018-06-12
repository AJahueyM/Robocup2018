#ifndef __RAMP_H_
#define __RAMP_H_
#include "Tile.h"

class Ramp {
public:
    Ramp();
    Ramp(Coord origin, uint8_t targetFloor);
    void setOrigin(Coord origin);
    Coord getOrigin() const;
    Coord getEnd() const;
    void setEnd(Coord end);
    uint8_t getTargetFloor() const;
    void setUsed(bool used);
    bool getUsed();

private:
    Coord origin;
    Coord end;
    uint8_t targetFloor = 0;
    bool used = false;
};
#endif
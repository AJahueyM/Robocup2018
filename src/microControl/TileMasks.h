#ifndef __TILEMASKS_H
#define __TILEMASKS_H
#include "Arduino.h"

const byte wallUpMask = B10000000;
const byte wallRightMask = B01000000;
const byte wallDownMask = B00100000;
const byte wallLeftMask = B00010000;
const byte isRampMask = B00001000;
const byte leftKitMask = B00000100;
const byte isBlackMask = B00000010;
const byte isCheckpointMask = B00000001;
const byte maskVisited = B10000000;
const byte hasBumpMask = B01000000;
#endif
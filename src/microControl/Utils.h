#ifndef __UTILS_HEADER_
#define __UTILS_HEADER_
#include <Arduino.h>
float mapD(float x, float in_min, float in_max, float out_min, float out_max);

float shortestAngleTurn(float currentAngle, float target);
#endif

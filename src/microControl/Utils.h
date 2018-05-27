#ifndef __UTILS_H_
#define __UTILS_H_
#include <Arduino.h>

enum Direction {
	Up,
	Down,
	Left,
	Right
};

enum Color {
  White,
  Black,
  Silver
};
float mapD(float x, float in_min, float in_max, float out_min, float out_max);

float shortestAngleTurn(float currentAngle, float target);
#endif

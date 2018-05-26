#include "Utils.h"

float mapD(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float shortestAngleTurn(float currentAngle, float target) {
    float angleToTurn = target - currentAngle;

    if (abs(angleToTurn) > 180) {
      if (currentAngle > 0) {
        if (angleToTurn > 0) {
          angleToTurn = angleToTurn - 180;
        } else {
          angleToTurn = angleToTurn + 360;
        }
      } else {
        if (angleToTurn > 0) {
          angleToTurn = angleToTurn - 360;
        } else {
          angleToTurn = angleToTurn + 180;
        }
      }
    }
    return angleToTurn;
}

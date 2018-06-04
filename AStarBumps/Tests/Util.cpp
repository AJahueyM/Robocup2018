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

int calculateDistance(Coord a, Coord b) {
	int dx = abs(a.getX() - b.getX());
	int dy = abs(a.getY() - b.getY()); 
	return (dx + dy); 
}

vector<Coord> getClosestFrom(vector <Coord>& vec, Coord start) {
	vector<Coord>  end;
	int distance = 0, minorDistance = 100;
	for (int i = 0; i < vec.size(); ++i) {
		Coord candidate = vec[i];
		distance = calculateDistance(start, candidate);

		if(minorDistance >= distance) {
      if(minorDistance > distance){
         minorDistance = distance;
         end.clear();
      }
			end.push_back(candidate);
		}

	}
	return end;
}
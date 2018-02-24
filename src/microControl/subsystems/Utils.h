#ifndef __UTILS_HEADER_
#define __UTILS_HEADER_

template<class T>
T mapD(T x, T in_min, T in_max, T out_min, T out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template<class T>
	T shortestAngleTurn(T currentAngle, T target) {
		double angleToTurn = target - currentAngle;

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

#endif
#ifndef __UTILS_HEADER_
#define __UTILS_HEADER_

double mapD(double x, double in_min, double in_max, double out_min, double out_max);

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
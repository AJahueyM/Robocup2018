#ifndef __DRIVETRAIN_HEADER_
#define __DRIVETRAIN_HEADER_

#include "Motor.h"

class DriveTrain{
private:
	Motor topRight, topLeft, lowRight, lowLeft;
	void setRightMotorsVelocity(double velocity);
	void setLeftMotorsVelocity(double velocity);
public:
	DriveTrain();
	void driveVelocity(double velocity);
	void turn(double rotation);
};




#endif
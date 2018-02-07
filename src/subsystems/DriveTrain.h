#ifndef __DRIVETRAIN_HEADER_
#define __DRIVETRAIN_HEADER_
#include <math.h>
#include <Encoder.h>

#include "Motor.h"
#include "Gyro.h"
#include "Utils.h"
class DriveTrain{
private:
	Motor topRight, topLeft, lowRight, lowLeft;
	Gyro gyro;
	Encoder enc;
	double wheelCircunference = 7.0 * M_PI, encCountsPerRev = 3630.0;


public:
	void setRightMotorsVelocity(double velocity);
	void setLeftMotorsVelocity(double velocity);
	DriveTrain();
	void driveVelocity(double velocity);
	void turn(double rotation);
	int getYaw();
	int getPitch();
	void resetYaw();
	void resetPitch();
	void resetAll();
	void turnToAngle(int angle);
	void driveStraight(double velocity, int angle);
	void driveDisplacement(double displacement, double velocity);
};




#endif
#ifndef __DRIVETRAIN_HEADER_
#define __DRIVETRAIN_HEADER_

#include "Motor.h"
#include "Gyro.h"
#include "Utils.h"
#include "Sharp.h"

class DriveTrain{
private:
	Motor topRight, topLeft, lowRight, lowLeft;
	Gyro gyro;
	Sharp frontSharp, rightSharp, leftSharp, backSharp;
	void setRightMotorsVelocity(double velocity);
	void setLeftMotorsVelocity(double velocity);
public:
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
	int getDistanceFront();
	int getDistanceLeft();
	int getDistanceRight();
	int getDistanceBack();

};




#endif
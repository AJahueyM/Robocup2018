#ifndef __DRIVETRAIN_HEADER_
#define __DRIVETRAIN_HEADER_
#include <math.h>
#include <Encoder.h>
#include "Motor.h"
#include "Gyro.h"
#include "Utils.h"
#include "Sharp.h"
#include "Button.h"

enum RobotFace {
	Back,
	Front
};

class DriveTrain{
private:
	Motor topRight, topLeft, lowRight, lowLeft;
	Gyro gyro;
	Sharp frontSharp, rightSharp, leftSharp, backSharp;
	Encoder enc;
	Button frontRLimitS, frontLLimitS, backRLimitS, backLLimitS;
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
	int getDistanceFront();
	int getDistanceLeft();
	int getDistanceRight();
	int getDistanceBack();
	void driveDisplacement(double displacement, double velocity);
	void alignWithWall(RobotFace faceToAlign);
};
#endif
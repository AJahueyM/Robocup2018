#ifndef __DRIVETRAIN_HEADER_
#define __DRIVETRAIN_HEADER_
#include <math.h>
#include <Encoder.h>
#include "Motor.h"
#include "Gyro.h"
#include "Utils.h"
#include "Sharp.h"
#include "Button.h"
#include "Dispenser.h"
#include <Adafruit_MLX90614.h>

enum RobotFace {
	Back,
	Front
};

class DriveTrain {
private:
	Motor topRight, topLeft, lowRight, lowLeft;
	Gyro gyro;
	Sharp frontSharp, rightSharp, leftSharp;
	Encoder enc;
	Button backRLimitS, backLLimitS;
	double wheelCircunference = 7.0 * M_PI, encCountsPerRev = 3300.0, heatDiferenceVictim = 5;
	long lastEncoderReading = 0, encoderReadRateMs = 100, lastHeatReading = 0, heatReadRateMs = 100;
	Adafruit_MLX90614 mlxR = Adafruit_MLX90614(0x5A);
	Adafruit_MLX90614 mlxL = Adafruit_MLX90614(0x55);
	Dispenser dispenser = Dispenser(6);

public:
	void setRightMotorsVelocity(double velocity);
	void setLeftMotorsVelocity(double velocity);
	void checkHeatDispense();

	DriveTrain();
	void driveVelocity(double velocity);
	void turn(double rotation);
	int getYaw();
	int getPitch();
	void resetYaw();
	void resetPitch();
	void resetAll();
	void turnToAngle(int angle);
	void driveStraight(int angle, double velocity);
	int getDistanceFront();
	int getDistanceLeft();
	int getDistanceRight();
	int getDistanceBack();
	void driveDisplacement(double displacement, int angle, double velocity);
	void alignWithWall(RobotFace faceToAlign);
};
#endif

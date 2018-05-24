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
#include "ColorSensor.h"
#include <Adafruit_MLX90614.h>
#include "LCD.h"

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
	double wheelCircunference = 7.0 * M_PI, encCountsPerRev = 3400.0, heatDiferenceVictim = 4, lastDisplacement = 0;
	long lastEncoderReading = 0, encoderReadRateMs = 16, lastHeatReading = 0, heatReadRateMs = 100;
	int led1Pin = 37, led2Pin = 39;
	Adafruit_MLX90614 mlxR = Adafruit_MLX90614(0x5A);
	Adafruit_MLX90614 mlxL = Adafruit_MLX90614(0x55);
	Dispenser dispenser = Dispenser::getInstance();
	ColorSensor colorSensor = ColorSensor();
	LCD& lcd = LCD::getInstance();
	bool lastDisplacementCompleted = false, interruptedColor = false, leftKit = false, shouldDispense = false;
	bool drivingWithDistance = false;
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
	void setYawOffset(int value);
	void turnToAngle(int angle);
	void driveStraight(int angle, double velocity);
	int getDistanceFront();
	int getDistanceLeft();
	int getDistanceRight();
	int getDistanceBack();
	void driveDisplacement(double displacement, int angle, double velocity, bool ignoreColorSensor = false);
	void alignWithWall(RobotFace faceToAlign);
	bool wasLastDisplacementCompleted();
	double getLastDisplacement();
	Color getTileColor();
	bool leftKitLastMovement();
	void setLeftKit(bool value);
	bool wasInterruptedColor();
	void setShouldDispense(bool value);
};
#endif

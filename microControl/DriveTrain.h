#ifndef __DRIVETRAIN_H_
#define __DRIVETRAIN_H_
#include <math.h>
#include "Absis.h"
#include <Encoder.h>
#include "Motor.h"
#include "Gyro.h"
#include "Utils.h"
//#include "Sharp.h"
#include <Ultrasonic.h>
#include "TOF.h"
#include "Button.h"
#include "Dispenser.h"
#include "ColorSensor.h"
#include <Adafruit_MLX90614.h>
#include "VisionSensor.h"
#include "LCD.h"
#include "Led.h"

enum RobotFace {
	Back,
	Front
};

class DriveTrain {
private:
	DriveTrain();
    DriveTrain &operator=(const DriveTrain &);
	Motor topRight, topLeft, lowRight, lowLeft;
	//Sharp  rightSharpFront, rightSharpBack, leftSharpFront, leftSharpBack;
	Ultrasonic rightUltra, leftUltra; // 39, 33, 31, 29
	TOF frontTof, backTof;
	Encoder encR, encL;
	Button backRLimitS, backLLimitS, frontRLimitS, frontLLimitS;
	double wheelCircunference = 7.0 * M_PI, encCountsPerRev = 3300, encCountsPerCm , heatVictim = 27, lastDisplacement = 0, cmsPitchRecord = 2;
	double kConstantDriveGyro = 0.05, kConstantDriveDistance = 0.07, kConstantTurn = 0.055;
	long alignTimeOut = 3000, turnTimeOut = 2000, moveWallTimeout = 3000, delayTurnCorrection = 500, delayCourseCorrection = 500, encCountsPitchRecord = 50, movementUpdateRate = 10, lastUpdatedMovement = 0, lastHeatReading = 0, lastEncoderReading = 0;
	short int angleCourseCorrection = 30;
	uint8_t heatDirection = 9;
	uint8_t wallDistanceSidesThresh = 20, wallDesiredDistance = 6.5;
	uint8_t encoderReadRateMs = 50, heatReadRateMs = 100;
	uint8_t led1Pin = 35, led2Pin = 37, blinkTimesVictimDetected = 50;
	Led leds;
	Adafruit_MLX90614 mlxL = Adafruit_MLX90614(0x5A);
	Adafruit_MLX90614 mlxR = Adafruit_MLX90614(0x55);
	ColorSensor colorSensor = ColorSensor::getInstance();
	Dispenser dispenser = Dispenser::getInstance();
	Gyro gyro = Gyro::getInstance();
	VisionSensor visionSensor = VisionSensor::getInstance();
	LCD& lcd = LCD::getInstance();
	Absis<int> pitchHistory;
	bool lastDisplacementCompleted = false, interruptedColor = false, leftKit = false, shouldDispense = true;
	bool drivingWithDistance = false;
public:
	static DriveTrain& getInstance(){
		static DriveTrain singletonInstance;
		return singletonInstance;
	};
	void setRightMotorsVelocity(double velocity);
	void setLeftMotorsVelocity(double velocity);
	void checkDispense();
	void calibrateHeatVictim();
	void blinkLeds(uint8_t times = 5);
	void driveVelocity(double velocity);
	void turn(double rotation);
	int getYaw();
	int getPitch();
	Absis<int> getPitchHistory();
	int getAngleDiffPitchHistory();
	int getAveragePitch();
	void resetYaw();
	void resetPitch();
	void resetAll();
	void setYawOffset(int value);
	void turnToAngle(int angle);
	void driveStraight(int angle, double velocity);
	int getDistanceFront();
	int getDistanceLeftFront();
	// int getDistanceLeftBack();
	int getDistanceRightFront();
	// int getDistanceRightBack();
	int getDistanceBack();
	int getDesiredWallDistance();
	void driveDisplacement(double displacement, int angle, double velocity, bool ignoreColorSensor = false);
	void alignWithWall(RobotFace faceToAlign);
	void moveDesiredDistanceToWall(double velocity);
	bool wasLastDisplacementCompleted();
	double getLastDisplacement();
	Color getTileColor();
	bool leftKitLastMovement();
	void setLeftKit(bool value);
	bool wasInterruptedColor();
	void setShouldDispense(bool value);
};
#endif

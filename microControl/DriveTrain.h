#ifndef __DRIVETRAIN_H_
#define __DRIVETRAIN_H_
#include <math.h>
#include "Absis.h"
#include <Encoder.h>
#include "Motor.h"
#include "Gyro.h"
#include "Utils.h"
#include "Sharp.h"
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
	Sharp  rightSharpFront, rightSharpBack, leftSharpFront, leftSharpBack;
	TOF frontTof, backTof;
	Encoder encR, encL;
	Button backRLimitS, backLLimitS, frontRLimitS, frontLLimitS;
	double wheelCircunference = 6.5 * M_PI, encCountsPerRev = 3576.0, encCountsPerCm , heatDiferenceVictim = 4, lastDisplacement = 0, cmsPitchRecord = 2;
	double kConstantDriveGyro = 1, kConstantDriveDistance = 1, kConstantTurn = 1;
	long turnTimeOut = 5000, delayTurnCorrection = 500, delayCourseCorrection = 1000, encCountsPitchRecord;
	short int angleCourseCorrection = 30;
	uint8_t wallDistanceSidesThresh = 15, wallDesiredDistance = 6.5;
	uint8_t lastEncoderReading = 0, encoderReadRateMs = 16, lastHeatReading = 0, heatReadRateMs = 100;
	uint8_t led1Pin = 35, led2Pin = 37, blinkTimesVictimDetected = 2;
	Led leds;
	Adafruit_MLX90614 mlxR = Adafruit_MLX90614(0x5A);
	Adafruit_MLX90614 mlxL = Adafruit_MLX90614(0x55);
	ColorSensor colorSensor = ColorSensor();
	Dispenser dispenser = Dispenser::getInstance();
	LCD lcd = LCD::getInstance();
	Gyro gyro = Gyro::getInstance();
	VisionSensor visionSensor = VisionSensor::getInstance();
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
	void blinkLeds(uint8_t times = 5);
	void driveVelocity(double velocity);
	void turn(double rotation);
	int getYaw();
	int getPitch();
	Absis<int> getPitchHistory();
	void resetYaw();
	void resetPitch();
	void resetAll();
	void setYawOffset(int value);
	void turnToAngle(int angle);
	void driveStraight(int angle, double velocity);
	int getDistanceFront();
	int getDistanceLeftFront();
	int getDistanceLeftBack();
	int getDistanceRightFront();
	int getDistanceRightBack();
	int getDistanceBack();
	int getDesiredWallDistance();
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

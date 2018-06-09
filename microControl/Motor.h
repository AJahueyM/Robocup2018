#ifndef __MOTOR_H_
#define __MOTOR_H_
#include <Adafruit_MotorShield.h>
#include <Arduino.h>
#include "Utils.h"
using namespace std;

class Motor{
private:
	static Adafruit_MotorShield AFMS; 
	Adafruit_DCMotor *myMotor;
	long updateRateMs = 10;
	long lastUpdate = 0;

public:
	Motor(uint8_t port);
	~Motor();
	void driveVelocity(double velocity);
};
#endif

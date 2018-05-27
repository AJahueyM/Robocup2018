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


public:
	Motor(int port);
	~Motor();
	void driveVelocity(double velocity);
};
#endif

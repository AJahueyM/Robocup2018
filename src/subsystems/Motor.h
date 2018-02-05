#ifndef __MOTOR_HEADER_
#define __MOTOR_HEADER_

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
	void driveVelocity(double velocity);
	void driveDisplacement(double displacement);

};

#endif

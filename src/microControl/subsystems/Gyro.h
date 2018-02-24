#ifndef __GYRO_HEADER_
#define __GYRO_HEADER_

#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
using namespace std;

class Gyro{
private:
	Adafruit_BNO055 bno = Adafruit_BNO055(55);
	double yawOffSet, pitchOffSet, lastYaw;
	int iTurnCounter = 0, iPitchCounter, lastPitch;

//Abiel w/h
public:
	Gyro();
	int getYaw();
	int getPitch();
	void resetYaw();
	void resetPitch();
	void resetAll();

};
#endif
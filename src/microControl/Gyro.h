#include <Adafruit_BNO055.h>

#ifndef __GYRO_HEADER_
#define __GYRO_HEADER_

#include <Adafruit_Sensor.h>
using namespace std;

class Gyro{
private:
	Adafruit_BNO055 bno = Adafruit_BNO055(55);
	int yawOffSet, pitchOffSet, yaw;
	int iTurnCounter = 0, iPitchCounter, lastPitch;
	long lastReadTime = 0, readRateMs = 100;

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

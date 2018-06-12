#ifndef __GYRO_H_
#define __GYRO_H_
#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
using namespace std;

class Gyro{
private:
	Gyro();
    Gyro &operator=(const Gyro &);

	Adafruit_BNO055 bno = Adafruit_BNO055(55);
	int yawOffSet, pitchOffSet, yaw = 0, pitch = 0;
	uint8_t iTurnCounter = 0, iPitchCounter, lastPitch ,readRateMs = 50;
	long lastReadTime = 0;

//Abiel w/h
public:
	static Gyro& getInstance(){
		static Gyro singletonInstance;
		return singletonInstance;
	};

	int getYaw();
	int getPitch();
	void resetYaw();
	void resetPitch();
	void resetAll();
	void setYawOffset(int value);
	int getYawOffset();
};
#endif

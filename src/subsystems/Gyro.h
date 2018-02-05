#include <Adafruit_BNO055.h>
#include <Adafruit_Sensor.h>
#include "subsystems/DriveTrain.h"

using namespace std;

class Gyro{
private:
	Adafruit_BNO055 bno = Adafruit_BNO055(55);
	double yawOffSet, pitchOffSet, lastYaw;
	int iTurnCounter, iPitchCounter, lastPitch;


public:
	Gyro();
	int getYaw();
	int getPitch();
	void resetYaw();
	void resetPitch();
	void resetAll();

};

#include "subsystems/Logic/Control/DriveTrain.h"

#define DEBUG

DriveTrain* driveTrain;

void setup(){
	#ifdef DEBUG
	Serial.begin(9600);
	#endif

	driveTrain = new DriveTrain();
}

void loop(){

	
	driveTrain->driveVelocity(0);
}
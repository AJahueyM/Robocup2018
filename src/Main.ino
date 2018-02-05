#include "subsystems/DriveTrain.h"

DriveTrain *driveTrain;
long time = millis();
void setup(){

	Serial.begin(9600);
	Serial.println("INICIAR PROG");

	driveTrain = new DriveTrain();
	
}

void loop(){
	//time = millis();
	//while(millis() - time < 2000){
		//driveTrain->driveStraight(1, 0);
	Serial.println(driveTrain->getYaw());
	//}
	//driveTrain->turnToAngle(driveTrain->getYaw() + 90);
}
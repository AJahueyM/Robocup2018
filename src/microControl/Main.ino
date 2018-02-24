#include "subsystems/DriveTrain.h"

DriveTrain* driveTrain;
bool ran;
void setup(){
	driveTrain = new DriveTrain();
}

void loop(){
	if(!ran){
		driveTrain->alignWithWall(Back);
		driveTrain->driveDisplacement(6.75, 0.75);
		ran = true;
	}

	if(driveTrain->getDistanceFront() > 30){

		driveTrain->driveDisplacement(30, 0.75);
	}else if(driveTrain->getDistanceRight() > 30){
		driveTrain->turnToAngle(driveTrain->getYaw() + 90);
		driveTrain->driveDisplacement(30,0.75);
	}else if(driveTrain->getDistanceLeft() > 30){

		driveTrain->turnToAngle(driveTrain->getYaw() - 90);
		driveTrain->driveDisplacement(30,0.75);
	}else{
		driveTrain->driveDisplacement(-30,0.75);
	}

	// 		//driveTrain->getYaw();

	// if(!ran){
	// 	//driveTrain->getYaw();
	// 	driveTrain->alignWithWall(Back);
	// 	driveTrain->driveDisplacement(6.75, 0.75);
	// 	driveTrain->driveDisplacement(30, 0.75);
	// 	driveTrain->turnToAngle(90);
	// 	driveTrain->driveDisplacement(30, 0.75);
	// 	driveTrain->turnToAngle(179);
	// 	driveTrain->driveDisplacement(30, 0.25);
	// }
	// ran = true;
}

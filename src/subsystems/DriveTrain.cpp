#include "DriveTrain.h"

DriveTrain::DriveTrain() : topRight(1), topLeft(3), lowRight(2), lowLeft(4){

}

///	MOTORS
void DriveTrain::setRightMotorsVelocity(double velocity){
	topRight.driveVelocity(velocity);
	lowRight.driveVelocity(velocity);
}

void DriveTrain::setLeftMotorsVelocity(double velocity){
	topLeft.driveVelocity(velocity);
	lowLeft.driveVelocity(velocity);
}

void DriveTrain::driveVelocity(double velocity){
	setRightMotorsVelocity(velocity);
	setLeftMotorsVelocity(velocity);
}

void DriveTrain::turn(double rotation){
	setRightMotorsVelocity(rotation * -1);
	setLeftMotorsVelocity(rotation); 
}

/// GYROSCOPE
int DriveTrain::getYaw(){
	return gyro.getYaw();
}
int DriveTrain::getPitch(){
	return gyro.getPitch();
}
void DriveTrain::resetYaw(){
	gyro.resetYaw();
}

void DriveTrain::resetPitch(){
	gyro.resetPitch();
}
void DriveTrain::resetAll(){
	gyro.resetAll();
}

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

void DriveTrain::turnToAngle(int angle){
	int angleError = shortestAngleTurn(getYaw(), angle);
    double outputMultiplier = mapD(abs(angleError), 0.0, 45, 0.0, 1.0);

	if (outputMultiplier > 1.0) {
		outputMultiplier = 1.0;
	} else if (outputMultiplier < 0.0) {
		outputMultiplier = 0.0;
	}

    while(angleError != 0){
    	angleError = shortestAngleTurn(getYaw(), angle);
		outputMultiplier = mapD(abs(angleError), 0.0, 45, 0.0, 1.0);

		if (outputMultiplier > 1.0) {
			outputMultiplier = 1.0;
		}else if (outputMultiplier < 0.0) {
			outputMultiplier = 0.0;
		}

		if (angleError > 0.0) {
			turn(outputMultiplier);
		} else if (angleError < 0.0) {
			turn(-outputMultiplier);
		}
    }
	turn(0);
}

void DriveTrain::driveStraight(double velocity, int angle){
 	double angleError = shortestAngleTurn(getYaw(), angle);
	double outputMultiplier = mapD(abs(angleError), 0.0, 15, 0.0, .8);

	if (outputMultiplier > .8) {
		outputMultiplier = .8;
	} else if (outputMultiplier < 0.0) {
		outputMultiplier = 0.0;
	}
	if (abs(angleError) > 0) {
		if (angleError > 0.0) {
			setLeftMotorsVelocity(velocity);
			setRightMotorsVelocity(velocity - (velocity * outputMultiplier));
		} else if (angleError < 0.0) {
			setLeftMotorsVelocity(velocity - (velocity * outputMultiplier));
			setRightMotorsVelocity(velocity);
		}
	} else {
		setLeftMotorsVelocity(velocity);
		setRightMotorsVelocity(velocity);
	}
}
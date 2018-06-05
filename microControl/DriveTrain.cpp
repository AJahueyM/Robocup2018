#include "DriveTrain.h"
///Constructor
DriveTrain::DriveTrain() : topRight(2), topLeft(3), lowRight(4), lowLeft(1), encR(19, 18),encL(16,17), 
	frontTof(42), backTof(49, 0x30), rightSharpFront(13), rightSharpBack(12),leftSharpFront(11), leftSharpBack(10),
	backRLimitS(43), backLLimitS(45),  frontRLimitS (4) , frontLLimitS(4), leds(led1Pin){
	Serial.println("DriveTrain initializing...");
	leds.addPin(led2Pin);
	leds.setState(false);
  	encR.write(0);
	encL.write(0);
	Serial.println("DriveTrain initialized");
	encCountsPerCm =	encCountsPerRev /  wheelCircunference; 
	encCountsPitchRecord = cmsPitchRecord * encCountsPerCm;
}

void DriveTrain::setRightMotorsVelocity(double velocity) {
	topRight.driveVelocity(velocity);
	lowRight.driveVelocity(velocity);
}

void DriveTrain::setLeftMotorsVelocity(double velocity) {
	topLeft.driveVelocity(velocity);
	lowLeft.driveVelocity(velocity);
}

void DriveTrain::blinkLeds(uint8_t times){
	leds.blink(times);
	leds.setState(false);
}

void DriveTrain::checkHeatDispense() {
	if(millis() - lastHeatReading > heatReadRateMs && shouldDispense){
		if (mlxL.readObjectTempC() - mlxL.readAmbientTempC() > heatDiferenceVictim) {
			blinkLeds(blinkTimesVictimDetected);
			turn(0);
			dispenser.dispenseDirection(DispenserDirection::left);
			leftKit = true;
		}
		if (mlxR.readObjectTempC() - mlxR.readAmbientTempC() > heatDiferenceVictim) {
			blinkLeds(blinkTimesVictimDetected);
			turn(0);
			dispenser.dispenseDirection(DispenserDirection::right);
			leftKit = true;
		}
		lastHeatReading = millis();
	}
}
void DriveTrain::driveVelocity(double velocity) {
	setRightMotorsVelocity(velocity);
	setLeftMotorsVelocity(velocity);
}

void DriveTrain::turn(double rotation) {
	setRightMotorsVelocity(rotation * -1);
	setLeftMotorsVelocity(rotation);
}

/// GYROSCOPE
int DriveTrain::getYaw() {
	return gyro.getYaw();
}

Absis<int> DriveTrain::getPitchHistory(){
	return pitchHistory;
}

int DriveTrain::getPitch() {
	return gyro.getPitch();
}
void DriveTrain::resetYaw() {
	gyro.resetYaw();
}

void DriveTrain::resetPitch() {
	gyro.resetPitch();
}
void DriveTrain::resetAll() {
	gyro.resetAll();
}

void DriveTrain::setYawOffset(int value){
	gyro.setYawOffset(value);
}

void DriveTrain::turnToAngle(int angle) {
	if (angle < -180) {
		do {
			angle += 180;
		} while (angle < -180);
	}
	if (angle > 180) {
		do {
			angle -= 180;
		}  while (angle > 180);
	}

	short int error = shortestAngleTurn(getYaw(), angle);
	long startTime = millis();

	while(error != 0 && millis() - startTime < turnTimeOut) {
		if(!leftKit && shouldDispense)
			checkHeatDispense();
		error = shortestAngleTurn(getYaw(), angle);
		double output = error * kConstantTurn;
		if(output > 1){
			output = 1;
		}else if(output < -1){
			output = -1;
		}
		turn(output);
	}
	turn(0);
	if(error != 0){
		if(error < 0){
			turn(0.75);
		}else if(error > 0){
			turn(-0.75);
		}
		delay(delayTurnCorrection);
		turnToAngle(angle);
	}

}

void DriveTrain::driveStraight(int angle, double velocity) {
	if(!leftKit && shouldDispense)
		checkHeatDispense();
	double multiplier;
	bool rightDistanceValid = (getDistanceRightFront() < wallDistanceSidesThresh) && (getDistanceRightBack() < wallDistanceSidesThresh);
	bool leftDistanceValid = (getDistanceLeftFront() < wallDistanceSidesThresh) && (getDistanceLeftBack() < wallDistanceSidesThresh);
	bool correctWithDistance = rightDistanceValid || leftDistanceValid;
	short int error = 0;
	if(correctWithDistance){
		drivingWithDistance = true;
		short int distanceError = 0;
		if(rightDistanceValid){
			if(getDistanceRightFront() != getDesiredWallDistance()){
				error = getDistanceFront() - getDesiredWallDistance();
			}else{
				error = getDistanceRightFront() - getDistanceRightBack();
			}

		}else{
			if(getDistanceLeftFront() != getDesiredWallDistance()){
				error = getDesiredWallDistance() - getDistanceLeftFront();
			}else{	
				error = getDistanceLeftBack() - getDistanceLeftFront();
			}
		}


		multiplier = error * kConstantDriveDistance;
	}else{
		drivingWithDistance = false;
		error = shortestAngleTurn(getYaw(), angle);
		multiplier = error * kConstantDriveGyro;
	}
	if(velocity < 0)
		error*=-1;

	if(error < 0){
		setRightMotorsVelocity(velocity);
		setLeftMotorsVelocity(velocity * multiplier);
	}else if (error > 0){
		setRightMotorsVelocity(velocity * multiplier);
		setLeftMotorsVelocity(velocity);
	}else{
		setRightMotorsVelocity(velocity);
		setLeftMotorsVelocity(velocity);
	}
}

void DriveTrain::driveDisplacement(double displacement, int angle, double velocity, bool ignoreColorSensor) {
	interruptedColor = false;
	lastDisplacementCompleted = false;
	if(velocity  == 0){
		return;
	}
	encR.write(0);
	encL.write(0);
	long startCountR = encR.read();
	long startCountL = encL.read();
	long encCountR = startCountR;
	long encCountL = startCountL;
	long averageMovement = 0;
	long toMove = displacement * encCountsPerCm;
	Color tileColor = White;
	Absis<int> pitchLog;
	long pitchRecordTarget = encCountsPitchRecord;
	int startDistance = getDistanceFront();
	while(abs(averageMovement) < toMove && tileColor != Black){

		if(velocity > 0 ){
			if(getDistanceFront() < getDesiredWallDistance()){
				return;
			}
		}else{
			if(getDistanceBack() < getDesiredWallDistance()){
				return;
			}
		}

		if(averageMovement > pitchRecordTarget){
			pitchLog.push_back(getPitch());
			pitchRecordTarget+=encCountsPitchRecord;
		}

		if(!ignoreColorSensor)
			tileColor = getTileColor();

		if(millis() - lastEncoderReading > encoderReadRateMs){
			encCountR = encR.read();
			encCountL = encL.read();
			lastEncoderReading = millis();
			averageMovement = (encCountL + encCountR) / 2;
			if(abs(getPitch()) < 2 && getPitch() < startDistance != 0){
				int newDistance = getDistanceFront() - startDistance;
				averageMovement = (averageMovement + (newDistance * encCountsPerCm)) / 2;
			}
		}
		driveStraight(angle, velocity);
		if(frontLLimitS.getState() || frontRLimitS.getState()){
				if(frontRLimitS.getState()){
					turnToAngle(getYaw() + angleCourseCorrection);
				}else{
					turnToAngle(getYaw() - angleCourseCorrection);
				}
				driveVelocity(-.5);
				delay(delayCourseCorrection);
				turnToAngle(0);
		}
	}
	pitchHistory =  pitchLog;
	if(tileColor == Black){
		interruptedColor = true;
	}else{
		lastDisplacementCompleted = true;
	}
	driveVelocity(0);
}

int DriveTrain::getDistanceFront() {
	return frontTof.getDistance();
}

int DriveTrain::getDistanceLeftFront() {
	return leftSharpFront.getDistance() - 4;
}

int DriveTrain::getDistanceLeftBack(){
	return leftSharpBack.getDistance() - 4;
}

int DriveTrain::getDistanceRightFront() {
	return rightSharpFront.getDistance() - 4;
}

int DriveTrain::getDistanceRightBack(){
	return rightSharpBack.getDistance() - 4;
}

int DriveTrain::getDistanceBack(){
	return backTof.getDistance();
}

void DriveTrain::alignWithWall(RobotFace faceToAlign) {
	Button *right, *left;
	double speed;
	switch (faceToAlign) {
	case Back:
		right = &backRLimitS;
		left = &backLLimitS;
		speed = -.75;
		break;

	case Front:
		right = &frontRLimitS;
		left = &frontLLimitS;
		speed = .75;
		break;
	}

	while (!right->getState() || !left->getState()) {
    Serial.print(right->getState());
    Serial.print(" ");
    Serial.println(left->getState());
		if (!right->getState())
			setRightMotorsVelocity(speed);
		else
			setRightMotorsVelocity(0);
		if (!left->getState())
			setLeftMotorsVelocity(speed);
		else
			setLeftMotorsVelocity(0);
	}
	driveVelocity(0);
}

Color DriveTrain::getTileColor(){
	return colorSensor.getColor();
}

bool DriveTrain::wasLastDisplacementCompleted(){
	return lastDisplacementCompleted;
}

double DriveTrain::getLastDisplacement(){
	return lastDisplacement;
}

bool DriveTrain::leftKitLastMovement(){
	return leftKit;
}

void DriveTrain::setLeftKit(bool value){
	leftKit = value;
}	

bool DriveTrain::wasInterruptedColor(){
	return interruptedColor;
}

void DriveTrain::setShouldDispense(bool value){
	shouldDispense = value;
}


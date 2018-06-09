#include "DriveTrain.h"
///Constructor
DriveTrain::DriveTrain() : topRight(4), topLeft(2), lowRight(1), lowLeft(3), encL(16, 17),encR(19,18), 
	frontTof(49, 0x32), backTof(42, 0x33), rightSharpFront(13), rightSharpBack(10),leftSharpFront(12), leftSharpBack(11),
	backRLimitS(4), backLLimitS(3),  frontRLimitS (45) , frontLLimitS(43), leds(led1Pin){
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
	topLeft.driveVelocity(-velocity);
	lowLeft.driveVelocity(-velocity);
}

void DriveTrain::blinkLeds(uint8_t times){
	leds.blink(times);
	leds.setState(false);
}

void DriveTrain::checkDispense() {
	if(!leftKit && shouldDispense){
		if(millis() - lastHeatReading > heatReadRateMs){
			if (mlxL.readObjectTempC() - mlxL.readAmbientTempC() > heatDiferenceVictim) {
				blinkLeds(blinkTimesVictimDetected);
				turn(0);
				dispenser.dispenseDirection(Left);
				leftKit = true;
			}
			if (mlxR.readObjectTempC() - mlxR.readAmbientTempC() > heatDiferenceVictim) {
				blinkLeds(blinkTimesVictimDetected);
				turn(0);
				dispenser.dispenseDirection(Right);
				leftKit = true;
			}
			lastHeatReading = millis();
		}

		VisionResponse visionResponse = visionSensor.getStatus();
		Direction dir = visionResponse.dir;
		if(visionResponse.victim != None){
			leftKit = true;
			blinkLeds(blinkTimesVictimDetected);
			switch(visionResponse.victim){
				case Harmed:{
					dispenser.dispenseDirection(dir, 2);
				}
				break;
				case Stable:{
					dispenser.dispenseDirection(dir);
				}
				break;
				case Unharmed:{

				}
				break;
				default: {
				}
				break;
			}

		}
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
	lcd.display(String("1"));
	// if (angle < -180) {
	// 	do {
	// 		angle += 180;
	// 	} while (angle < -180);
	// }
	// if (angle > 180) {
	// 	do {
	// 		angle -= 180;
	// 	}  while (angle > 180);
	// }
	lcd.display(String("2"));

	short int error =  angle - getYaw();// shortestAngleTurn(getYaw(), angle);
	long startTime = millis();
	lcd.display(String("3"));

	while(abs(error) > 1 && millis() - startTime < turnTimeOut) {
		lcd.display(String("4"));
		checkDispense();
		error = angle - getYaw();// shortestAngleTurn(getYaw(), angle);
		lcd.display(String("5"));
		double output = error * kConstantTurn;

		output = output > 1 ? 1 : output;
		output = output < -1 ? -1 : output;
		output = output < 0.225 && output >  0 ?  0.225 : output;
		output = output < 0 && output >  -0.225 ?  -0.225 : output;

		lcd.display(String("6"));
		lcd.display(String("7"));
		turn(output);
		lcd.display(String("8"));
	}
	turn(0);
	if(abs(error) > 10){
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
	// cout << "R: " << encR.read() << "\t L: " << encL.read() << endl;
	// String str;
	// str.concat("R:");
	// str.concat(encR.read());
	// str.concat(" L:");
	// str.concat(encL.read());
	// lcd.display(str);
	checkDispense();
	double multiplier;
	bool rightDistanceValid = (getDistanceRightFront() < wallDistanceSidesThresh) && (getDistanceRightBack() < wallDistanceSidesThresh);
	bool leftDistanceValid = (getDistanceLeftFront() < wallDistanceSidesThresh) && (getDistanceLeftBack() < wallDistanceSidesThresh);
	bool correctWithDistance = rightDistanceValid || leftDistanceValid;
	double error = 0;
	if(correctWithDistance){
		drivingWithDistance = true;
		double distanceError = 0;
		if(rightDistanceValid){
			// if(getDistanceRightFront() != getDesiredWallDistance()){
				error = getDistanceRightFront() - getDesiredWallDistance();
			// }else{
			// 	error = getDistanceRightFront() - getDistanceRightBack();
			// }
		}else{
			// if(getDistanceLeftFront() != getDesiredWallDistance()){
				error = getDesiredWallDistance() - getDistanceLeftFront();
			// }else{	
			// 	error = getDistanceLeftBack() - getDistanceLeftFront();
			// }
		}


		multiplier = error * kConstantDriveDistance;
	}else{
		drivingWithDistance = false;
		error = angle - getYaw();
		multiplier = error * kConstantDriveGyro;
	}

	multiplier = abs(multiplier);


	if(multiplier >  .25){
		multiplier = .25;
	}
	if(multiplier < 0){
		multiplier = 0;
	}
	if(velocity < 0)
		error*=-1;

	if(error < 0){
		setRightMotorsVelocity(velocity);
		setLeftMotorsVelocity(velocity * (1 -multiplier));
	
	}else if (error > 0){
		setRightMotorsVelocity(velocity * (1 - multiplier));
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
	while(abs(averageMovement) <= toMove && tileColor != Black){
		lcd.display(String(averageMovement / encCountsPerCm));
		if(velocity > 0 ){
			if(getDistanceFront() < getDesiredWallDistance()){
				turn(0);
				return;
			}
		}else{
			if(getDistanceBack() < getDesiredWallDistance()){
				turn(0);
				return;
			}
		}

		if(averageMovement > pitchRecordTarget){
			pitchLog.push_back(getPitch());
			pitchRecordTarget+=encCountsPitchRecord;
		}

		if(!ignoreColorSensor)
			///tileColor = getTileColor();

		if(millis() - lastEncoderReading > encoderReadRateMs){
			//encCountR = encR.read();
			encCountL = encL.read();
			lastEncoderReading = millis();
			averageMovement = encCountL;
			// if(abs(getPitch()) < 2 && startDistance != 0){
			// 	int newDistance = getDistanceFront() - startDistance;
			// 	averageMovement = (averageMovement + (newDistance * encCountsPerCm)) / 2;
			// }
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
	lcd.display(String("IM DONE BEATCH"));

	turn(0);

	pitchHistory =  pitchLog;
	if(tileColor == Black){
		interruptedColor = true;
	}else{
		lastDisplacementCompleted = true;
	}
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
	return  backTof.getDistance();
}

int DriveTrain::getDesiredWallDistance(){
	return wallDesiredDistance;
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


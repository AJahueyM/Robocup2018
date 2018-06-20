#include "DriveTrain.h"
///Constructor
DriveTrain::DriveTrain() : topRight(2), topLeft(1), lowRight(4), lowLeft(3), encL(19, 18),encR(16,17), 
	frontTof(49, 0x32), backTof(42, 0x33) /*, rightSharpFront(13), rightSharpBack(10),leftSharpFront(12), leftSharpBack(11)*/,
	backRLimitS(4), backLLimitS(3),  frontRLimitS (45) , frontLLimitS(43), leds(led1Pin) ,rightUltra(39, 33), leftUltra(31, 29){
	Serial.println("DriveTrain initializing...");
	leds.addPin(led2Pin);
	leds.setState(false);
  	encR.write(0);
	encL.write(0);
	Serial.println("DriveTrain initialized");
	encCountsPerCm =	encCountsPerRev /  wheelCircunference; 
	encCountsPitchRecord = cmsPitchRecord * encCountsPerCm;
	heatVictim = EEPROM.read(heatDirection);
}

void DriveTrain::setRightMotorsVelocity(double velocity) {
	topRight.driveVelocity(velocity);
	lowRight.driveVelocity(velocity);
}

void DriveTrain::setLeftMotorsVelocity(double velocity) {
	String str;
	str.concat(frontRLimitS.getState());
	str.concat(frontLLimitS.getState());
	lcd.display(str);
	topLeft.driveVelocity(-velocity);
	lowLeft.driveVelocity(velocity);
}

void DriveTrain::blinkLeds(uint8_t times){
	leds.blink(times);
	leds.setState(false);
}

void DriveTrain::checkDispense() {
	if((!leftKit && shouldDispense) && (getAngleDiffPitchHistory() < 5)){
		if(millis() - lastHeatReading > heatReadRateMs){
			if ( mlxL.readObjectTempC() >= heatVictim * .9) {
				turn(0);
				blinkLeds(blinkTimesVictimDetected);
				dispenser.dispenseDirection(Left);
				leftKit = true;
			}else if (mlxR.readObjectTempC() >= heatVictim * .9) {
				turn(0);
				blinkLeds(blinkTimesVictimDetected);
				dispenser.dispenseDirection(Right);
				leftKit = true;
			}
			lastHeatReading = millis();
		}

		// VisionResponse visionResponse = visionSensor.getStatus();
		// Direction dir = visionResponse.dir;
		// if(visionResponse.victim != None){
		// 	leftKit = true;
		// 	blinkLeds(blinkTimesVictimDetected);
		// 	switch(visionResponse.victim){
		// 		case Harmed:{
		// 			dispenser.dispenseDirection(dir, 2);
		// 		}
		// 		break;
		// 		case Stable:{
		// 			dispenser.dispenseDirection(dir);
		// 		}
		// 		break;
		// 		case Unharmed:{

		// 		}
		// 		break;
		// 		default: {
		// 		}
		// 		break;
		// 	}

		// }
	}
}

void DriveTrain::calibrateHeatVictim(){
	heatVictim = mlxL.readObjectTempC();
	EEPROM.write(heatDirection, (uint8_t) heatVictim);
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

int DriveTrain::getAngleDiffPitchHistory(){
	Absis<int> pitchHistory = getPitchHistory();
	int lowestAngle = 300, highestAngle = -300;

	for(int i = 0; i < pitchHistory.size(); ++i){
		int angle = pitchHistory[i];
		if(angle < lowestAngle){
			lowestAngle = angle;
		}
		if(angle > highestAngle){
			highestAngle = angle;
		}
	}
	int angleDiff = abs(highestAngle - lowestAngle);
	if(angleDiff > 200)
		angleDiff = 0;
	return angleDiff;
}

int DriveTrain::getAveragePitch(){
	int sum = 0;
	for(int i = 0; i < pitchHistory.size(); ++i){
		sum += pitchHistory[i];
	}	
	return sum/pitchHistory.size();
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

	float error =  shortestAngleTurn(getYaw(), angle); //angle - getYaw();// shortestAngleTurn(getYaw(), angle);

	for(int i = 0; i < 5; ++i){

		long startTime = millis();
		bool leftKitReset = false;
		while(millis() - startTime < turnTimeOut && abs(error) > 1) {
			if(millis() - lastUpdatedMovement > movementUpdateRate){
				lastUpdatedMovement = millis();
				checkDispense();

				if(leftKit && !leftKitReset){
					startTime = millis();
					leftKitReset = true;
				}
				error = shortestAngleTurn(getYaw(), angle); // angle - getYaw();// shortestAngleTurn(getYaw(), angle);
				double output = error * kConstantTurn;

				output = output > 1 ? 1 : output;
				output = output < -1 ? -1 : output;
				output = output < 0.3 && output >  0 ?  0.3 : output;
				output = output < 0 && output >  -0.3 ?  -0.3 : output;
				turn(output);
			}
		}
		turn(0);
		if(abs(error) > 10.0){
			if(error < 0.0){
				turn(0.75);
			}else if(error > 0.0){
				turn(-0.75);
			}
			delay(delayTurnCorrection);
			turn(0);
		}else{
			break;
		}
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
	bool rightDistanceValid = (getDistanceRightFront() <= wallDistanceSidesThresh) && (getDistanceRightFront() > 2) ;
	bool leftDistanceValid = (getDistanceLeftFront() <= wallDistanceSidesThresh)  && (getDistanceLeftFront() > 2) ;
	bool correctWithDistance = rightDistanceValid || leftDistanceValid;
	double error = 0;
	int angleError = abs(shortestAngleTurn(getYaw(), angle));
	if(correctWithDistance && angleError < 10){
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
		error = shortestAngleTurn(getYaw(), angle);
		multiplier = error * kConstantDriveGyro;
	}

	multiplier = abs(multiplier);


	if(multiplier >  1){
		multiplier = 1;
	}
	if(multiplier < 0){
		multiplier = 0;
	}
	if(velocity < 0)
		error*=-1;

	if(error < 0){
		setRightMotorsVelocity(velocity);
		setLeftMotorsVelocity(velocity * (1.0 - multiplier));
	
	}else if (error > 0){
		setRightMotorsVelocity(velocity * (1.0 - multiplier));
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
	bool expandedMovement = false, moving = true;
	int distanceRepeatedCounter = 0;
	int distanceCounterLimit = 25;
	int distanceBeforeStuck = 0;
	int lastDistance = 0;
	int distanceThresh = 1, distanceTreshMovingRestored = 5;
	while(abs(averageMovement) <= toMove){
		if(millis() - lastUpdatedMovement > movementUpdateRate){
			lastUpdatedMovement = millis();
		
			int currentDistance = 0;
			if(getDistanceFront() < 500){
				currentDistance = getDistanceFront();
			}else if(getDistanceBack() < 500){
				currentDistance = getDistanceBack();
			}else{
				distanceRepeatedCounter = 0;
			}
			if( abs(lastDistance - currentDistance) < distanceThresh){

				distanceRepeatedCounter++;
			}
			if(!moving){
				if(lastDistance - currentDistance > distanceTreshMovingRestored)
					moving  = true;
			}
			lastDistance = currentDistance;

			if(distanceRepeatedCounter > distanceCounterLimit){
				turn(0);
				turn(-.75);
				delay(500);
				turn(.75);
				delay(500);
				turn(0);
				distanceRepeatedCounter = 0;
				expandedMovement = false;
				moving = false;
				toMove *= 1.2;
			}	

			if(velocity > 0 ){
				if(getDistanceFront() < getDesiredWallDistance() && getDistanceFront() != 0){
					lastDisplacementCompleted = true;
					turn(0);
					return;
				}
			}else{
				if(getDistanceBack() < getDesiredWallDistance() &&  getDistanceBack() != 0){
					lastDisplacementCompleted = true;
					turn(0);
					return;
				}
			}

			if(averageMovement > pitchRecordTarget){
				pitchLog.push_back(getPitch());
				pitchRecordTarget+=encCountsPitchRecord;
			}

			if(!ignoreColorSensor)
				tileColor = getTileColor();

			if(tileColor == Black){
				break;
			}

			if(millis() - lastEncoderReading > encoderReadRateMs){
				//encCountR = encR.read();
				if(moving)
					encCountL = encL.read();
				else {
					encCountL = averageMovement;
					encL.write(averageMovement);
				}

				lastEncoderReading = millis();
				averageMovement = encCountL;
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
					turnToAngle(angle);
			}
			if(getPitch() >  10 && !expandedMovement){
				expandedMovement = true;
				toMove *= 1.10;
				if(getPitch() >  15){
					toMove *= 1.10;
				}
			}

			if(getPitch() > 30){
				turn(0);
				if(velocity > 0)
					driveVelocity(-.3);
				else
					driveVelocity(.3);
				delay(750);
				turn(0);
			}

		}
	}

	turn(0);
	turnToAngle(angle);

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
	//return leftSharpFront.getDistance() - 4;
	return leftUltra.Ranging(CM) - 4;
}

// int DriveTrain::getDistanceLeftBack(){
// 	return leftSharpBack.getDistance() - 4;
// }

int DriveTrain::getDistanceRightFront() {
	//return rightSharpFront.getDistance() - 4;
	return rightUltra.Ranging(CM) - 4;
}

// int DriveTrain::getDistanceRightBack(){
// 	return rightSharpBack.getDistance() - 4;
// }

int DriveTrain::getDistanceBack(){
	return  backTof.getDistance();
}

int DriveTrain::getDesiredWallDistance(){
	return wallDesiredDistance;
}

void DriveTrain::alignWithWall(RobotFace faceToAlign) {

	double speed = -.4;

	long startTime = millis();
	while ((!backRLimitS.getState() || !backLLimitS.getState() )&& millis() - startTime < alignTimeOut )  {
		if(millis() - lastUpdatedMovement > movementUpdateRate){
			lastUpdatedMovement = millis();		
			if(getPitch() > 30){
				turn(0);
				if(speed > 0)
					driveVelocity(-.3);
				else
					driveVelocity(.3);
				delay(1000);
				turn(0);
			}

			if (!backRLimitS.getState())
				setRightMotorsVelocity(speed);
			else
				setRightMotorsVelocity(0);
			if (!backLLimitS.getState())
				setLeftMotorsVelocity(speed);
			else
				setLeftMotorsVelocity(0);
		}
	}
	driveVelocity(0);
}

void DriveTrain::moveDesiredDistanceToWall(double velocity){
	long startTime = millis();
	while((getDistanceFront() < 18 &&  getDistanceFront() != getDesiredWallDistance() ) && getDistanceFront() != 0) {
			if(millis() - startTime > moveWallTimeout)
				break;

			if(getDistanceFront() > getDesiredWallDistance()){
				driveVelocity(velocity);
			}else{
				driveVelocity(-velocity);
			}
	}
	turn(0);
	startTime = millis();
	while((getDistanceBack() < 18 &&  getDistanceBack() != getDesiredWallDistance() ) && getDistanceBack() != 0) {
		if(millis() - startTime > moveWallTimeout)
			break;
		if(getDistanceBack() > getDesiredWallDistance()){
			driveVelocity(-velocity);
		}else{
			driveVelocity(velocity);
		}
	}
	turn(0);
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


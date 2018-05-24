#include "DriveTrain.h"
///Constructor
DriveTrain::DriveTrain() : topRight(2), topLeft(3), lowRight(4), lowLeft(1), enc(19, 18),
	frontSharp(12), rightSharp(11), leftSharp(13),
	backRLimitS(51), backLLimitS(53) {
	Serial.println("DriveTrain initializing...");
	pinMode(led1Pin, OUTPUT);
	pinMode(led2Pin, OUTPUT);
	digitalWrite(led1Pin, LOW);
	digitalWrite(led2Pin, LOW);
  	enc.write(0);

	Serial.println("DriveTrain initialized");

}

///	MOTORS
void DriveTrain::setRightMotorsVelocity(double velocity) {
	topRight.driveVelocity(velocity);
	lowRight.driveVelocity(velocity);
}

void DriveTrain::setLeftMotorsVelocity(double velocity) {
	topLeft.driveVelocity(velocity);
	lowLeft.driveVelocity(velocity);
}

void DriveTrain::checkHeatDispense() {
	if(millis() - lastHeatReading > heatReadRateMs && shouldDispense){
		if (mlxL.readObjectTempC() - mlxL.readAmbientTempC() > heatDiferenceVictim) {
			digitalWrite(led1Pin, HIGH);
			digitalWrite(led2Pin, HIGH);
			turn(0);
			dispenser.dispenseDirection(DispenserDirection::left);
			leftKit = true;
		}
		if (mlxR.readObjectTempC() - mlxR.readAmbientTempC() > heatDiferenceVictim) {
			digitalWrite(led1Pin, HIGH);
			digitalWrite(led2Pin, HIGH);
			turn(0);
			dispenser.dispenseDirection(DispenserDirection::right);
			leftKit = true;
		}
		lastHeatReading = millis();
	}
	digitalWrite(led1Pin, LOW);
	digitalWrite(led2Pin, LOW);
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
	double angleError = shortestAngleTurn(getYaw(), angle);
	double outputMultiplier = mapD(fabs(angleError), 0.0, 45.0, 0.1, 1.0);
	long start = millis();
	while (abs(angleError) > 0 && millis() - start < 2000) {
		//lcd.display(String(angleError));
		if(!leftKit && shouldDispense)
			checkHeatDispense();
		angleError = shortestAngleTurn(getYaw(), angle);
		outputMultiplier = mapD(fabs(angleError), 0.0, 45.0, 0.1, 1.0);
		if (outputMultiplier > .8) {
			outputMultiplier = 0.8;
		} else if (outputMultiplier < 0.1) {
			outputMultiplier = 0.1;
		}

		if (angleError > 0.0) {
			turn(outputMultiplier);
		} else if (angleError < 0.0) {
			turn(-outputMultiplier);
		}
	}
	turn(0);
}

void DriveTrain::driveStraight(int angle, double velocity) {
	if(!leftKit && shouldDispense)
		checkHeatDispense();

	double angleError = shortestAngleTurn(getYaw(), angle);
	//lcd.display(String(angleError));
	double outputMultiplier = mapD(abs(angleError), 0.0, 7.0, 0.0, 1);
	
	if (outputMultiplier > 1) {
		outputMultiplier = 1;
	} else if (outputMultiplier < 0.0) {
		outputMultiplier = 0.0; // RE ESTABLECIENDO EL VALOR DE OUTPUTMULTIPLIER CUANDO VELOCIDAD ES NEGATIVA
	}
	if(getDistanceRight() < 8 || getDistanceLeft() < 8){
		drivingWithDistance = true;
		if(getDistanceRight() < 8){
			setLeftMotorsVelocity(velocity * .8);
			setRightMotorsVelocity(velocity);
		}else{
			setLeftMotorsVelocity(velocity);
			setRightMotorsVelocity(velocity * .8);
		}
	}else if (abs(angleError) > 0) {
		drivingWithDistance = false;
		if (angleError > 0.0) {
			if(velocity > 0){
				setLeftMotorsVelocity(velocity);
				setRightMotorsVelocity(velocity - (velocity * outputMultiplier));				
			}else{
				setLeftMotorsVelocity(velocity - (velocity * outputMultiplier));
				setRightMotorsVelocity(velocity);		
			}
		} else if (angleError < 0.0) {
			if(velocity > 0){
				setLeftMotorsVelocity(velocity - (velocity * outputMultiplier));
				setRightMotorsVelocity(velocity);
			}else{
				setLeftMotorsVelocity(velocity);
				setRightMotorsVelocity(velocity  - (velocity * outputMultiplier));
			}
		}
	} else {
		setLeftMotorsVelocity(velocity);
		setRightMotorsVelocity(velocity);

	}
}

void DriveTrain::driveDisplacement(double displacement, int angle, double velocity, bool ignoreColorSensor) {
	lastDisplacement = displacement;
	enc.write(0);
	long startCount = enc.read();
	startCount = abs(startCount);
	long toMove = (abs(displacement) / wheelCircunference) * encCountsPerRev;
	long encCount = startCount;
	lastEncoderReading  = millis();
	interruptedColor = false;
	Color tileColor = White;
	lastDisplacementCompleted = false;
	int correctionCounter = 0;
	bool expandedMovement = false;
	while ((abs(encCount - startCount)  < toMove && getDistanceFront() > 7 )  && tileColor != Black) {
		if(!ignoreColorSensor)
			tileColor = getTileColor();
		if (millis() - lastEncoderReading > encoderReadRateMs) {
			encCount = enc.read();
			encCount = abs(encCount);
			lastEncoderReading = millis();
		}
		Serial.println(abs(encCount - startCount));
		driveStraight(angle, velocity);


		// if((abs(abs(getYaw()) - abs(angle)) > 10) && !drivingWithDistance){
		// 	correctionCounter++;
		// 	toMove -= abs(encCount - startCount);
		// 	toMove *= 1.15;
		// 	long start = millis();
		// 	while(millis() - start < 500 && correctionCounter < 3){ // millis() - toMove < 500
		// 		driveVelocity(-1);
		// 	}
		// 	if(correctionCounter >= 3){
		// 		while(millis() - start < 1000){ // millis() - toMove < 500
		// 			driveVelocity(1);
		// 		}	
		// 		correctionCounter = 0;		
		// 	}
		// 	turnToAngle(angle);
		// 	enc.write(0);
		// 	startCount = enc.read();
		// }

		if((abs(getPitch()) > 5 && abs(getPitch()) < 15 ) && !expandedMovement){
			expandedMovement = true;
			toMove *= 1.1;
			velocity *= 1.25;
		}
	}
	if((abs(encCount - startCount)  >= toMove * 0.75) && tileColor != Black ) {
		lastDisplacementCompleted = true;
	}

	if(tileColor == Black){
		interruptedColor = true;
	}
	driveVelocity(0);
}

int DriveTrain::getDistanceFront() {
	return frontSharp.getDistance();
}

int DriveTrain::getDistanceLeft() {
	return leftSharp.getDistance();
}

int DriveTrain::getDistanceRight() {
	return rightSharp.getDistance();
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
	resetYaw();
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


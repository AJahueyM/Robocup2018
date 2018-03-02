#include "DriveTrain.h"
///Constructor
DriveTrain::DriveTrain() : topRight(1), topLeft(2), lowRight(4), lowLeft(3), enc(19, 18),
	frontSharp(13), rightSharp(12), leftSharp(14),
	backRLimitS(31), backLLimitS(47) {
	Serial.println("DriveTrain initializing...");
	Serial.println("DriveTrain initialized");
	enc.write(0);

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
	if(millis() - lastHeatReading > heatReadRateMs){
		if (mlxL.readObjectTempC() - mlxL.readAmbientTempC() > heatDiferenceVictim) {
			dispenser.dispenseDirection(DispenserDirection::left);
		}
		if (mlxR.readObjectTempC() - mlxR.readAmbientTempC() > heatDiferenceVictim) {
			dispenser.dispenseDirection(DispenserDirection::right);
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
	double outputMultiplier = mapD(fabs(angleError), 0.0, 15.0, 0.0, 1.0);

	while (abs(angleError) > 0) {
		checkHeatDispense();
		angleError = shortestAngleTurn(getYaw(), angle);
		outputMultiplier = mapD(fabs(angleError), 0.0, 15.0, 0.0, 1.0);
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
	checkHeatDispense();

	double angleError = shortestAngleTurn(getYaw(), angle);
	double outputMultiplier = mapD(abs(angleError), 0.0, 15.0, 0.0, .8);
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

void DriveTrain::driveDisplacement(double displacement, int angle, double velocity) {
	enc.write(0);
	long startCount = enc.read();
	startCount = abs(startCount);
	long toMove = (abs(displacement) / wheelCircunference) * encCountsPerRev;
	long encCount = startCount;
	lastEncoderReading  = millis();

	while (abs(encCount - startCount)  < toMove && getDistanceFront() > 15 ) {
		if (millis() - lastEncoderReading > encoderReadRateMs) {
			encCount = enc.read();
			encCount = abs(encCount);
			lastEncoderReading = millis();
		}
		driveStraight(angle, velocity);
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

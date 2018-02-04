#include "Motor.h"


Motor::Motor(int port){
	AFMS.begin();
	Serial.println("ANTES DE ESO");

	if(port < 5 && port > 0)
		myMotor = AFMS.getMotor(port);
	else
		myMotor = AFMS.getMotor(1);

	Serial.println("DESPUES DE");
}

void Motor::driveVelocity(double velocity){
	double output = mapD(fabs(velocity), 0.0,1.0,0.0,255.0);
	Serial.println(output);

	myMotor->setSpeed(output);
	if(velocity > 0.0){
		myMotor->run(FORWARD);
		Serial.println("ENTRO AL FORWARD");
	}else if(velocity < 0.0){
		myMotor->run(BACKWARD);
		Serial.println("ENTRO AL BACKWARD");
	}else{
		myMotor->run(RELEASE);
		Serial.println("RELEASE OF DEATH");
	}
}
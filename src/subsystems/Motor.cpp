#include "Motor.h"
Adafruit_MotorShield Motor::AFMS = Adafruit_MotorShield();

Motor::Motor(int port){
	AFMS.begin();
	if(port < 5 && port > 0)
		myMotor = AFMS.getMotor(port);
	else
		myMotor = AFMS.getMotor(1);
}

void Motor::driveVelocity(double velocity){
	double output = mapD(fabs(velocity), 0.0,1.0,0.0,255.0);
	Serial.println(output);

	myMotor->setSpeed(output);
	if(velocity > 0.0){
		myMotor->run(FORWARD);
	}else if(velocity < 0.0){
		myMotor->run(BACKWARD);
	}else{
		myMotor->run(RELEASE);
	}
}


Motor::~Motor(){
	delete myMotor;
}
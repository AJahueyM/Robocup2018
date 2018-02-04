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

double map(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Motor::driveVelocity(double velocity){
	double output = map(fabs(velocity), 0.0,1.0,0.0,255.0);
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
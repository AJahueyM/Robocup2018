#include "subsystems/Gyro.h"

Gyro *gyro;

void setup(){

	Serial.begin(9600);
	Serial.println("INICIAR PROG");

	gyro = new Gyro();

	
}

void loop(){
 	Serial.println(gyro->getYaw());
}
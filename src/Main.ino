#include "subsystems/Gyro.h"

Gyro *gyro;

void setup(){

	Serial.begin(9600);
	Serial.println("INICIAR PROG");

	gyro = new Gyro();

	
}

void loop(){
 	Serial.print(gyro->getPitch());
 	Serial.print("       ");
 	Serial.println(gyro->getYaw());
}
#include <Encoder.h>
#include <math.h>
#include "subsystems/DriveTrain.h"

DriveTrain *driveTrain;
void setup(){
	Serial.begin(9600);
	Serial.println("INICIAR PROG");
	driveTrain = new DriveTrain();	
}


void loop(){
	
}
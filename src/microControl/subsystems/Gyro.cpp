#include "Gyro.h"

Gyro::Gyro(){
	Serial.println("BNO Initializing...");
	if(!bno.begin()){
		Serial.println("NO BNO 055 DETECTED");
		while(1);
		}
	bno.setExtCrystalUse(true);
	delay(1000);
	double startTime = millis();
	while(getYaw() != 0){
		resetAll();

	}
	Serial.println("BNO Initialized");
}


int Gyro::getYaw(){
	imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

	double yaw = euler.x() - yawOffSet; ///TODO= REVISAR ESTO, Primero fue 40, luego fue 160, no se que esta pasando
	if(yaw > 180){
		yaw -=360;
	}

 //  	int finalAngle = baseAngle;
 // 	int vueltas  = baseAngle / 180;

 //  	if(baseAngle > 180){
 //      if(vueltas % 2 == 0){
 //        finalAngle = (abs(baseAngle) % 180);
 //      }else{
 //        finalAngle = -180 + (abs(baseAngle) % 180);
 //      }
 //  	}else if(baseAngle < -180){
 //    	if(vueltas % 2 == 0){
	//       	finalAngle = -1*(abs(baseAngle) % 180);  
	//     }else{
	//       finalAngle = 180 - (abs(baseAngle)%180);
	//     }
	// }

	return yaw;
}

int Gyro::getPitch(){
	imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

	double pitch = abs(abs(euler.z()) - pitchOffSet);

	return pitch;

}

void Gyro::resetYaw(){
	imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  	yawOffSet = euler.x();	

}

void Gyro::resetPitch(){
	imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  	pitchOffSet = abs(euler.z());	
}

void Gyro::resetAll(){
	resetYaw();
	resetPitch();
}

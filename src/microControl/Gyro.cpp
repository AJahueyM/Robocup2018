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
  	yawOffSet = 0;
	pitchOffSet = 0;
	Serial.println("BNO Initialized");
}


int Gyro::getYaw(){

	if(millis() - lastReadTime > readRateMs){
		imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
		int i = euler.x();
		bool debug = false;
		if(debug){
			Serial.print("Before= ");
			Serial.print(i);
		}

		yaw = i - yawOffSet; ///TODO= REVISAR ESTO, Primero fue 40, luego fue 160, no se que esta pasando
		if(yaw > 180)
			yaw -= 360;

		if(yaw < -180)
			yaw+=360;

		if(debug){
			Serial.print("\t After= ");
			Serial.print(yaw);

			Serial.print("\t Offset");
			Serial.println(yawOffSet);
		}
		lastReadTime = millis();
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
	int pitch = abs(euler.y() - pitchOffSet);
	return pitch;

}

void Gyro::resetYaw(){
	imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  	yawOffSet = euler.x();	


}

void Gyro::resetPitch(){
	imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  	pitchOffSet = abs(euler.y());	
}

void Gyro::resetAll(){
	resetYaw();
	resetPitch();
}
void Gyro::setYawOffset(int value){
	yawOffSet = value;
}

int Gyro::getYawOffset(){
	return yawOffSet;
}
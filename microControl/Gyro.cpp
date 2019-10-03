#include "Gyro.h"

Gyro::Gyro(){
	Serial.println("BNO Initializing...");
	if(!bno.begin(bno.OPERATION_MODE_IMUPLUS)){
		Serial.println("NO BNO 055 DETECTED");
		//while(1);
	}
	//bno.setExtCrystalUse(true);
	delay(1000);
	double startTime = millis();
	resetAll();
	Serial.println("BNO Initialized");
}
void Gyro::updateValues(){
	if(millis() - lastReadTime > readRateMs){
		lastReadTime = millis();
		imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
		int rawX = euler.x();

		yaw = rawX - yawOffSet;
		if(yaw > 180)
			yaw -= 360;

		if(yaw < -180)
			yaw+=360;

		int pitchNew = abs(euler.y() + pitchOffSet);
		
		pitch = pitchNew;
	}	
}


int Gyro::getYaw(){
  	updateValues();
	return yaw;
}

int Gyro::getPitch(){
	updateValues();
	return pitch;

}

void Gyro::resetYaw(){
	imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  	yawOffSet = euler.x();	


}

void Gyro::resetPitch(){
	imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
  	pitchOffSet = -euler.y();	
}

void Gyro::resetAll(){
	resetYaw();
	delay(readRateMs * 2);
	resetPitch();
}
void Gyro::setYawOffset(int value){
	yawOffSet = value;
}

int Gyro::getYawOffset(){
	return yawOffSet;
}
#include "Gyro.h"

Gyro::Gyro(){

	if(!bno.begin()){
		Serial.print("NO BNO 055 DETECTED");
		while(1);
		}

	bno.setExtCrystalUse(true);
	delay(5000);

	resetAll();

}


int Gyro::getYaw(){
	imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

	double yaw = euler.x(); ///TODO FIX THIS NONSENSE, por algun motivo siempre inicia en un valor de 40
     
     if(yaw - lastYaw < -320 ){
     	iTurnCounter++;
     }else if(yaw - lastYaw > 320){
     	iTurnCounter--;
     }

    int baseAngle = iTurnCounter * 360 + yaw - yawOffSet;

  	int finalAngle = baseAngle;
 	int vueltas  = baseAngle / 180;

  	if(baseAngle > 180){
      if(vueltas % 2 == 0){
        finalAngle = (abs(baseAngle) % 180);
      }else{
        finalAngle = -180 + (abs(baseAngle) % 180);
      }
  	}else if(baseAngle < -180){
    	if(vueltas % 2 == 0){
	      	finalAngle = -1*(abs(baseAngle) % 180);  
	    }else{
	      finalAngle = 180 - (abs(baseAngle)%180);
	    }
	}

    lastYaw = yaw;
	return finalAngle;
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

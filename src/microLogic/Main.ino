#include "subsystems/Requester.h"
void setup (){
  	Serial.begin(9600); 
	Requester& req = Requester::getInstance();
  	Serial.println(req.requestSensorValue(DistanceRight));
 
} 


void loop (){
	Requester& req = Requester::getInstance();
  	Serial.println(req.requestSensorValue(DistanceRight));
} 
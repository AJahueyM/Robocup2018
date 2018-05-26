#include "Requester.h"

Requester::Requester() {

}

int Requester::requestSensorValue(Sensor sensor){
	if(currentSensor!= sensor){
		megaAknowledged = false;
	}

	if(!megaAknowledged){
		lastTime = millis();
		char response;

		response = comms.sendMessage(Mega, "\tdr");
		while(response != 'p'){
			response = comms.sendMessage(Mega, "\tdr");

			long currentTime = millis();
			if(currentTime - lastTime > requestTimeout){
				Serial.println("Could not read from Mega, check wiring");
				lastTime = currentTime;
				break;
			}
		}
		if(response == 'p'){
			megaAknowledged = true;
			Serial.println("Aknowledged");
		}
	}
	return comms.pullMessage(Mega);
}
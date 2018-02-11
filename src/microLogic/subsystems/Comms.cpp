#include "Comms.h"

Comms::Comms(){
	pinMode(MEGA_SS_PIN, OUTPUT);
	digitalWrite(MEGA_SS_PIN, HIGH);

	pinMode(RASP_SS_PIN, OUTPUT);
	digitalWrite(RASP_SS_PIN, HIGH);
	SPI.begin ();
}

void Comms::sendMessage(Chip target, String message){
	uint8_t ss = target == Rasp ? RASP_SS_PIN : MEGA_SS_PIN;

	digitalWrite(MEGA_SS_PIN, HIGH);
	digitalWrite(RASP_SS_PIN, HIGH);
	if(!message.endsWith(String('\n'))) // Si el ultimo caracter del mensage no tiene '\n' 
		message.concat('\n');

	digitalWrite(ss, LOW);
	char c;
	for (const char * p = message.c_str(); c = *p; ++p){
	    SPI.transfer (c);
	    delay(2);
	}
	digitalWrite(ss, HIGH);
}
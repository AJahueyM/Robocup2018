#include "subsystems/Comms.h"

Comms* comms;
void setup (){
  Serial.begin(9600);  
  comms = new Comms();
} 


void loop (){
    comms->sendMessage(Mega, "Hello MOTO!");
  if(comms->receiveMessage() == 'd'){
    Serial.println("IT RESPONDED");    
    comms->sendMessage(Mega, "Hello MOTO!");
  }
} 
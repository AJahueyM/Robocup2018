#include "Dispenser.h"
#include <Arduino.h>


Dispenser::Dispenser(){
	servoDispenser.attach(pin);
	amountAvailable = 12;
	outputCenter = 100;
	outputLeft = 178;
	outputRight = 11;
 	servoDispenser.write(outputCenter);
}

void Dispenser::setAmountAvailable(uint8_t iAmount){
	amountAvailable = iAmount;
}
uint8_t Dispenser::getAmountAvailable(){
	return amountAvailable;
}
uint8_t Dispenser::updateAmountAvailable(uint8_t drop){
	amountAvailable -= drop;
	return amountAvailable;
}

void Dispenser::dispenseDirection(Direction direction, uint8_t amount){
	if(direction == Left){
		for(int i =0; i < amount; i++){
			if(kitsAvailable()){
				servoDispenser.write(outputCenter);
				delay(500);
				servoDispenser.write(outputLeft);
				delay(500);
				servoDispenser.write(outputCenter);
				updateAmountAvailable(1);
				delay(500);
			}
			
		}
	}else if(direction == Left){
		for(int i =0; i < amount; i++){
			if(kitsAvailable()){
				servoDispenser.write(outputCenter);
				delay(500);
				servoDispenser.write(outputRight);
				delay(500);
				servoDispenser.write(outputCenter);
				updateAmountAvailable(1);
				delay(500);
			}
		}
	}else{
		servoDispenser.write(outputCenter);
	}
	
}

bool Dispenser::kitsAvailable(){
	if(getAmountAvailable() > 0)
		return true;
	else 
		return false;
}

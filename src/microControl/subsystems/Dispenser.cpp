#include "Dispenser.h"
#include <Arduino.h>


Dispenser::Dispenser(int pin, int startAmount){
	servoDispenser.attach(pin);
	amountAvailable = startAmount;
	outputCenter = 46;
	outputLeft = 84;
	outputRight = 13;
}

void Dispenser::setAmountAvailable(int iAmount){
	amountAvailable = iAmount;
}
int Dispenser::getAmountAvailable(){
	return amountAvailable;
}
int Dispenser::updateAmountAvailable(int drop){
	amountAvailable -= drop;
	return amountAvailable;
}

void Dispenser::dispenseDirection(DispenserDirection direction, int amount){
	if(direction == left){
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
	}else if(direction == right){
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
		servoDispenser.write(90);
	}
	
}

bool Dispenser::kitsAvailable(){
	if(getAmountAvailable() > 0)
		return true;
	else 
		return false;
}
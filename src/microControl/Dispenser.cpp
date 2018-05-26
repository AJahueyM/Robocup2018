#include "Dispenser.h"
#include <Arduino.h>


Dispenser::Dispenser(){
	servoDispenser.attach(pin);
	amountAvailable = 12;
	outputCenter = 75;
	outputLeft = 115;
	outputRight = 40;
 	servoDispenser.write(outputCenter);
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
		servoDispenser.write(outputCenter);
	}
	
}

bool Dispenser::kitsAvailable(){
	if(getAmountAvailable() > 0)
		return true;
	else 
		return false;
}

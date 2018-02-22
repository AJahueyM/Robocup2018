#include "Dispenser.h"
#include <Arduino.h>


Dispenser::Dispenser(int pin, int startAmount){
	servoDispenser.attach(pin);
	amountAvailable = startAmount;
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
				servoDispenser.write(45);
				delay(500);
				servoDispenser.write(90);
				updateAmountAvailable(1);
				delay(500);
			}
			
		}
	}else if(direction == right){
		for(int i =0; i < amount; i++){
			if(kitsAvailable()){
				servoDispenser.write(135);
				delay(500);
				servoDispenser.write(90);
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
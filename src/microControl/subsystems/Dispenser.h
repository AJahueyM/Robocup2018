#include <Servo.h>

enum dispenserDirection{
right, left, center;
}

class dispenser {
private:
	int amountAvailable, startAmount = 12;
public:
	dispenser(int pin);
	dispenser(int startAmount);
	void setAmountAvailable(int iAmount);
	int getAmountAvailable();
	int updateAmountAvailable(int drop);
	void dispenserDirection(dispenserDirection direction, int amount);
	bool kitsAvailable(int startAmount, int amountAvailable);
}; 

dispenser::dispenser(int pin){
}
dispenser::dispenser(int startAmount){
	amountAvailable = startAmount;
}
void dispenser::setAmountAvailable(int iAmount){
	amountAvailable = iAmount;
}
int dispenser::getAmountAvailable(){
	return amountAvailable;
}
int dispenser::updateAmountAvailable(int drop){
	amountAvailable -= drop;
	return amountAvailable;
}

void dispenser::dispenserDirection(dispenserDirection direction, int amount){
	if(kitsAvailable){
		if(direction == left){
			for(int i =0; i< amount; i++){
				servoDispenserwrite(45);
				delay(1000);
				servoDispenser.write(90);
				servoDispenser.updateAmountAvailable(drop);
			}
		}else if(direction == right){
			for(int i =0; i<amount; i++){
				servoDispenser.write(135);
				delay(1000);
				servoDispenser.write(90);
				servoDispenser.updateAmountAvailable(int drop)
			}
		}else if(direction == center)
			servoDispenser.write(90);
	}
}

bool dispenser::kitsAvailable(int amountAvailable){
	if(amountAvailable > 0)
		return true;
	else 
		return false;
}

#include <Servo.h>

enum DispenserDirection{
right, left, center
};

class Dispenser {
private:
	int amountAvailable, startAmount = 12;
	Servo servoDispenser;
public:
	Dispenser(int pin);
	Dispenser(int startAmount);
	void setAmountAvailable(int iAmount);
	int getAmountAvailable();
	int updateAmountAvailable(int drop);
	void dispenseDirection(DispenserDirection direction, int amount);
	bool kitsAvailable(int startAmount, int amountAvailable);
}; 

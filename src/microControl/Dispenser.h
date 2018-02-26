#include <Servo.h>

enum DispenserDirection{
right, left, center
};

class Dispenser {
private:
	int amountAvailable, startAmount;
	int outputCenter, outputLeft, outputRight;
	Servo servoDispenser;
public:
	Dispenser(int pin, int startAmount = 12);
	void setAmountAvailable(int iAmount);
	int getAmountAvailable();
	int updateAmountAvailable(int drop);
	void dispenseDirection(DispenserDirection direction, int amount = 1);
	bool kitsAvailable();
}; 


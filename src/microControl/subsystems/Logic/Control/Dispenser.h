#include <Servo.h>

enum DispenserDirection{
right, left, center
};

class Dispenser {
private:
	int amountAvailable, startAmount;
	Servo servoDispenser;
	Dispenser(int pin, int startAmount);
	Dispenser &operator=(const Dispenser &);	
public:
	static Dispenser &getInstance(int pin, int startAmount = 12) {
		static Dispenser singletonInstance(pin, startAmount);
		return singletonInstance;
	}	
	void setAmountAvailable(int iAmount);
	int getAmountAvailable();
	int updateAmountAvailable(int drop);
	void dispenseDirection(DispenserDirection direction, int amount);
	bool kitsAvailable();
}; 


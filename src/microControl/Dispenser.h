#include <Servo.h>

enum DispenserDirection{
right, left, center
};

class Dispenser {
private:
	int amountAvailable;
	int outputCenter, outputLeft, outputRight;
	int pin = 6;
	Servo servoDispenser;
 	Dispenser();
    Dispenser &operator=(const Dispenser &);
public:
	static Dispenser& getInstance(){
		static Dispenser singletonInstance;
		return singletonInstance;
	};
	void setAmountAvailable(int iAmount);
	int getAmountAvailable();
	int updateAmountAvailable(int drop);
	void dispenseDirection(DispenserDirection direction, int amount = 1);
	bool kitsAvailable();
}; 


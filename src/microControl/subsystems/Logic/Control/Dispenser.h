<<<<<<< HEAD:src/microControl/subsystems/Dispenser.h
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

=======
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

>>>>>>> bf1da665426bebe720148ca177b1b148067eeac9:src/microControl/subsystems/Logic/Control/Dispenser.h

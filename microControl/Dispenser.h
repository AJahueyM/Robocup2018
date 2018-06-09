#ifndef __DISPENSER_H_
#define __DISPENSER_H_
#include <Servo.h>
#include "Utils.h"

class Dispenser {
private:
	uint8_t amountAvailable;
	uint8_t outputCenter, outputLeft, outputRight;
	uint8_t pin = 6;
	//Servo servoDispenser;
 	Dispenser();
    Dispenser &operator=(const Dispenser &);
public:
	static Dispenser& getInstance(){
		static Dispenser singletonInstance;
		return singletonInstance;
	};
	void setAmountAvailable(uint8_t iAmount);
	uint8_t getAmountAvailable();
	uint8_t updateAmountAvailable(uint8_t drop);
	void dispenseDirection(Direction direction, uint8_t amount = 1);
	bool kitsAvailable();
}; 
#endif

#ifndef __ROBOT_HEADER_
#define __ROBOT_HEADER_
#include "DriveTrain.h"
#include "Button.h"
#include "Dispenser.h"
#include "LCD.h"

class Robot{
public:
	static Robot &getInstance() {
		static Robot singletonInstance;
		return singletonInstance;
	}
	DriveTrain& driveTrain = DriveTrain::getInstance();
	Dispenser& dispenser = Dispenser::getInstance(11);
private:
	Robot &operator=(const Robot &);	
	Robot();
};

#endif
#include <LiquidCrystal_I2C.h>


class LCD{
private:
	String message;
	LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
public:
	LCD();
	void display(String message);
	String getMessage();
};


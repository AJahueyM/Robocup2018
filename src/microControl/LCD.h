#ifndef __LCD_HEADER_
#define __LCD_HEADER_
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class LCD{
private:
	String message;
	LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  	long updateTimeMs = 64, lastUpdate = 0;
 	LCD();
    LCD &operator=(const LCD &);
public:
	static LCD& getInstance(){
		static LCD singletonInstance;
		return singletonInstance;
	};

	void display(String message);
	String getMessage();
};

#endif


#ifndef __LCD_H_
#define __LCD_H_
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>

class LCD{
private:
	String message;
	LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x3F, 16, 2);
  	uint8_t updateTimeMs = 64;
	long lastUpdate = 0;
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


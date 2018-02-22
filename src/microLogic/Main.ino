#include <LiquidCrystal_I2C.h>
#include <Wire.h> 
#include <Arduino.h>

// Set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup(){
	// initialize the LCD
	lcd.begin();
	String message = "1ola2ola3ola4ola5ola6ola7ola8ola";
	// Turn on the blacklight and print a message.
	lcd.backlight();

	if(message.length() > 16){
		lcd.print(message.substring(0,16));
		lcd.setCursor(0,1);
		lcd.print(message.substring(16,32));
    }else{
    	lcd.print(message);
    }
}

void loop(){
	// Do nothing here...
}
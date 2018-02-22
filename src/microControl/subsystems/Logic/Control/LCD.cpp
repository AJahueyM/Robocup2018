#include "LCD.h"
LCD::LCD(){
	lcd.begin();
	lcd.backlight();	
}

void LCD::display(String message){
	Serial.println("LCD initializing...");
	if(message.length() > 16){
		lcd.print(message.substring(0,16));
		lcd.setCursor(0,1);
		lcd.print(message.substring(16,32));
    }else{
    	lcd.print(message);
    }

    lcd.setCursor(0,0);

    Serial.println("LCD initialized");
}

String LCD::getMessage(){
	return message;
}
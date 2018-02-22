#include <LiquidCrystal_I2C.h>

class LCD{
private:
	String message;

public:
	LCD();
	void display(String message);
	String getMessage();
};


LCD::LCD(){
	lcd.begin();
	lcd.backlight();	
}

void LCD::display(String message){

	if(message.length() > 16){
		lcd.print(message.substring(0,16));
		lcd.setCursor(0,1);
		lcd.print(message.substring(16,32));
    }else{
    	lcd.print(message);
    }
}

String LCD::getMessage(){
	return message;
}
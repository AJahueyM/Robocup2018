#include "LCD.h"
LCD::LCD(){
	Serial.println("LCD initializing...");

	lcd.begin();
	lcd.backlight();	
  Serial.println("LCD initialized");

}

void LCD::display(String message){
  if(millis() - lastUpdate > updateTimeMs){
      lcd.clear();
      if(message.length() > 16){
      lcd.print(message.substring(0,16));
      lcd.setCursor(0,1);
      lcd.print(message.substring(16,32));
      }else{
        lcd.print(message);
      }

    lastUpdate = millis();
    }

}

String LCD::getMessage(){
	return message;
}

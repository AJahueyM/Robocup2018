#include "Cerebrum.h"
#include <MemoryFree.h>
#include <ArduinoSTL.h>

DriveTrain* driveTrain;
Cerebrum* cerebrum;
using namespace std;

void setup() {
  LCD lcd = LCD::getInstance();

  lcd.display(String("Booting up..."));
  Serial.begin(9600);
  driveTrain = &DriveTrain::getInstance();
  cerebrum = &Cerebrum::getInstance(*driveTrain);
	lcd.display(String("Robot booted up"));
  
  Button colorCalButton(30);

  if(colorCalButton.getState()){
    ColorSensor colorSensor = ColorSensor::getInstance();

    delay(200);
    while(colorCalButton.getState()){
      delay(10);
      lcd.display("RELEASE CAL BUTTON");
    }

    while(!colorCalButton.getState()){
      delay(10);
      lcd.display("PLACE ON WHITE AND PRESS");
    }
    colorSensor.calibrateWhite();
    delay(200);

    while(!colorCalButton.getState()){
      delay(10);
      lcd.display(colorSensor.getColor());
    }
    delay(200);

    while(!colorCalButton.getState()){
      delay(10);
      lcd.display("PLACE ON BLACK AND PRESS");
    }
    colorSensor.calibrateBlack();
      delay(200);

    while(!colorCalButton.getState()){
      delay(10);
      lcd.display(colorSensor.getColor());
    }
    delay(200);

    while(!colorCalButton.getState()){
      delay(10);
      lcd.display("PLACE ON SILVER AND PRESS");
    }
    colorSensor.calibrateSilver();
    delay(200);

    while(!colorCalButton.getState()){
      delay(10);
      lcd.display(colorSensor.getColor());
    }
    delay(200);

    colorSensor.saveProfiles();


    delay(200);

    while(!colorCalButton.getState()){
      delay(10);
      lcd.display("PLACE ON VICTIM");
    }
    driveTrain->calibrateHeatVictim();
    delay(200);

  }

  delay(200);
  while(!colorCalButton.getState()){
    delay(10);
    lcd.display("PLACE ON STARTING POS");
  }


  driveTrain->blinkLeds(10);

  cerebrum->start();


  cerebrum->run();
  delay(500);
  lcd.display("DONE!!! gay:)");

}

void loop() {


  // LCD lcd = LCD::getInstance();
  // String str;
  // str.concat(analogRead(13));
  // str.concat("   ");
  // str.concat(analogRead(12));

  
}

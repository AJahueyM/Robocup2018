#include "Cerebrum.h"
#include <MemoryFree.h>
#include <ArduinoSTL.h>
#include "TOF.h"

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
      lcd.display("PLACE ON BLACK AND PRESS");
    }
    colorSensor.calibrateBlack();
    delay(200);

    while(!colorCalButton.getState()){
      delay(10);
      lcd.display("PLACE ON SILVER AND PRESS");
    }
    colorSensor.calibrateSilver();
    delay(200);

    while(!colorCalButton.getState()){
      delay(10);
      lcd.display("PLACE ON SILVER AND PRESS");
    }
  }

  delay(200);
  while(!colorCalButton.getState()){
    delay(10);
    lcd.display("PLACE ON STARTING POS");
  }

  driveTrain->blinkLeds(10);

  
  // driveTrain->driveDisplacement(30,0, .5);
  // driveTrain->turnToAngle(-90);
  // driveTrain->driveDisplacement(30,-90, .5);

  // TOF tof(TOF::validPin[0]);
  // tof.getDistance();

  cerebrum->start();


  cerebrum->run();
}

void loop() {
  LCD lcd = LCD::getInstance();
 // cout << driveTrain->getTileColor() << endl;
  //driveTrain->setRightMotorsVelocity(.25);
  // driveTrain->blinkLeds();
  // delay(500);
  //driveTrain->getYaw();
  //cout <<  << endl;
  //driveTrain->turnToAngle(0);
  // String str;
  // str.concat(driveTrain->getDistanceFront());
  // str.concat(" ");
  // str.concat(driveTrain->getDistanceRightFront());
  // str.concat(" ");
  // str.concat(driveTrain->getDistanceLeftFront());
  // str.concat(" ");
  // str.concat(driveTrain->getDistanceBack());
  //lcd.display(str);
  // cout << "F: " << driveTrain->getDistanceFront() << " B: " << driveTrain->getDistanceBack() << " RF: " << driveTrain->getDistanceRightFront() << " RB: " << driveTrain->getDistanceRightBack() << " LF: " << driveTrain->getDistanceLeftFront() << " LB: "<< driveTrain->getDistanceLeftBack() << endl;
  lcd.display("DONE!!! :)");
}

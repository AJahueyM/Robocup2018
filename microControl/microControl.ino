#include "Cerebrum.h"
#include <MemoryFree.h>
#include <ArduinoSTL.h>

DriveTrain* driveTrain;
Cerebrum* cerebrum;
using namespace std;
    uint8_t sLetterPin = 10, hLetterPin = 11, uLetterPin = 12, directionLetterPin = 13;

void setup() {
  LCD lcd = LCD::getInstance();

  lcd.display(String("Booting up..."));
  Serial.begin(9600);
  driveTrain = &DriveTrain::getInstance();
  cerebrum = &Cerebrum::getInstance(*driveTrain);
	lcd.display(String("Robot booted up"));
  pinMode(sLetterPin, INPUT);
  pinMode(hLetterPin, INPUT);
  pinMode(uLetterPin, INPUT);
  pinMode(directionLetterPin, INPUT);

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

  }

  delay(200);
  while(!colorCalButton.getState()){
    delay(10);
    lcd.display("PLACE ON STARTING POS");
  }

  delay(500);
  while(!colorCalButton.getState()){
    delay(10);
    lcd.display(driveTrain->getPitch());
  }
  driveTrain->blinkLeds(10);

  cerebrum->start();


  cerebrum->run();
  delay(500);
  lcd.display("DONE!!! gay:)");
}

void loop() {
  // String str;
  // str.concat(digitalRead(sLetterPin));
  // str.concat(digitalRead(hLetterPin));
  // str.concat(digitalRead(uLetterPin));
  // str.concat(digitalRead(directionLetterPin));
  // lcd.display(str);

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
  // lcd.display(str);
  // cout << "F: " << driveTrain->getDistanceFront() << " B: " << driveTrain->getDistanceBack() << " RF: " << driveTrain->getDistanceRightFront() << " RB: " << driveTrain->getDistanceRightBack() << " LF: " << driveTrain->getDistanceLeftFront() << " LB: "<< driveTrain->getDistanceLeftBack() << endl;

  //lcd.display(driveTrain->getPitch());
}

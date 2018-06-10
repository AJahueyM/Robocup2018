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
  delay(200);
	lcd.display(String("Robot booted up"));
  
  Button button1(4);
	while(!button1.getState())
		delay(10);

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
 // LCD& lcd = LCD::getInstance();
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
}

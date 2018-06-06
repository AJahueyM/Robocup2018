#include "Cerebrum.h"
#include <MemoryFree.h>
#include <ArduinoSTL.h>
#include "TOF.h"

DriveTrain* driveTrain;
Cerebrum* cerebrum;

using namespace std;

void setup() {
  Serial.begin(9600);
  driveTrain = &DriveTrain::getInstance();
  cerebrum = &Cerebrum::getInstance(*driveTrain);


  driveTrain->blinkLeds(10);
  // TOF tof(TOF::validPin[0]);
  // tof.getDistance();
  // cerebrum->start();
  // cerebrum->run();
}

void loop() {
  LCD& lcd = LCD::getInstance();
 // cout << driveTrain->getTileColor() << endl;
  driveTrain->driveVelocity(.5);
  //driveTrain->setRightMotorsVelocity(.25);
  // driveTrain->blinkLeds();
  // delay(500);
  //driveTrain->getYaw();
  //cout <<  << endl;
  String str;
  str.concat(driveTrain->getDistanceFront());
  str.concat(" ");
  str.concat(driveTrain->getDistanceBack());
  str.concat(" ");
  str.concat(driveTrain->getDistanceRightFront());
  str.concat(" ");
  str.concat(driveTrain->getDistanceLeftFront());
  lcd.display(str);
  //cout << "F: " << driveTrain->getDistanceFront() << " B: " << driveTrain->getDistanceBack() << " RF: " << driveTrain->getDistanceRightFront() << " RB: " << driveTrain->getDistanceRightBack() << " LF: " << driveTrain->getDistanceLeftFront() << " LB: "<< driveTrain->getDistanceLeftBack() << endl;
}

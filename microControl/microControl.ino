#include "Cerebrum.h"
#include <MemoryFree.h>
#include <ArduinoSTL.h>
#include "TOF.h"

DriveTrain* driveTrain;
Cerebrum* cerebrum;

//Button limit1(43), limit2(45), limit3(3), limit4(4), button1(28), button2(30);
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
  // cout << driveTrain->getTileColor() << endl;
  // driveTrain->driveVelocity(.5);
  // driveTrain->blinkLeds();
  // delay(500);
}

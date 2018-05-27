#include "Cerebrum.h"

DriveTrain* driveTrain;
Cerebrum* cerebrum;

void setup() {
  driveTrain = &DriveTrain::getInstance(); 
  cerebrum = &Cerebrum::getInstance(*driveTrain);
  cerebrum->start();
}

void loop() {
  cerebrum->update();
}

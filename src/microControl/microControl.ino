// #include "Cerebrum.h"
#include <MemoryFree.h>
#include "Cerebrum.h"
DriveTrain* driveTrain;
Cerebrum* cerebrum;

using namespace std;

void setup() {
  Serial.begin(115200);

  driveTrain = &DriveTrain::getInstance();
  cerebrum = &Cerebrum::getInstance(*driveTrain);

  cerebrum->start();

  cerebrum->update();

}

void loop() {

}

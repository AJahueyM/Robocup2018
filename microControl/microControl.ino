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

  TOF tof(TOF::validPin[0]);
  tof.getDistance();
}

void loop() {

}

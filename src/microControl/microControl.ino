// #include "Cerebrum.h"
#include <MemoryFree.h>
//#include "Cerebrum.h"
//#include "Adafruit_VL53L0X.h"
#include "Gyro.h"
#include <ArduinoSTL.h>
// DriveTrain* driveTrain;
// Cerebrum* cerebrum;

// Button *button1, *button2, *button3, *button4;
// Led *led1, *led2;
//Sharp* sharp1, *sharp2, *sharp3, *sharp4;
using namespace std;

void setup() {
  Serial.begin(9600);
  Gyro gyro = Gyro::getInstance();

}

void loop() {
  Gyro gyro = Gyro::getInstance();
  cout << gyro.getYaw() << endl;
}

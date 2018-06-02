// #include "Cerebrum.h"
#include <MemoryFree.h>
//#include "Cerebrum.h"
#include "Adafruit_VL53L0X.h"
// DriveTrain* driveTrain;
// Cerebrum* cerebrum;

// Button *button1, *button2, *button3, *button4;
// Led *led1, *led2;
//Sharp* sharp1, *sharp2, *sharp3, *sharp4;
using namespace std;
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void setup() {
  Serial.begin(9600);
  // sharp1 = new Sharp(10);
  // sharp2 = new Sharp(11);
  // sharp3 = new Sharp(12);
  // sharp4 = new Sharp(13);

   if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // led1 = new Led(35);
  // led2 = new Led(37);

  // pinMode(30, OUTPUT);
  // pinMode(32, OUTPUT);

  // driveTrain = &DriveTrain::getInstance();
  // cerebrum = &Cerebrum::getInstance(*driveTrain);

  // cerebrum->start();

  // cerebrum->run();
  // String str("ITZEL DELTA");
  // LCD::getInstance().display(str);
  // button1 = new Button(43);
  // button2 = new Button(45);
  // button3 = new Button(3);
  // button4 = new Button(4);
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
    
  //Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    ///Serial.print("Distance (mm): "); 
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println(" out of range ");
  }
  // led1->blink(5);
  // led2->blink(5);
  // digitalWrite(32, HIGH);
  // digitalWrite(30, HIGH);

 //cout <<(int) sharp1->getDistance() << "," <<  (int) sharp2->getDistance()  << "," << (int) sharp3->getDistance() << "," << (int) sharp4->getDistance() << endl;
}

#include "subsystems/Motor.h"

using namespace std;

Motor *right1; // = Motor(2);
Motor *right2;
Motor *left1;
Motor *left2;

void setup(){

Serial.begin(9600);
Serial.println("INICIAR PROG");
right2 = new Motor(2);
right1 = new Motor(1);
left2 = new Motor(4);
left1 = new Motor(3);

}


void loop(){

right1->driveVelocity(0);
right2->driveVelocity(0);
left1->driveVelocity(0);
left2->driveVelocity(0);

}
#include <Adafruit_MLX90614.h>
#include "DriveTrain.h"
#include "Dispenser.h"
#include "LCD.h"

DriveTrain* driveTrain;
LCD* lcd;
Dispenser* dispenser;
int distanceThresh = 7;
double targetAngle;
bool ran;
enum Direction{
  Left,
  Right
};

Adafruit_MLX90614 mlxR = Adafruit_MLX90614(0x5A);
Adafruit_MLX90614 mlxL = Adafruit_MLX90614(0x55);

void setup(){
  Serial.begin(9600);
	driveTrain = new DriveTrain();
  dispenser = new Dispenser(6);
  lcd = new LCD();
  targetAngle = 0;

}


void code(){
  if(!ran){
    driveTrain->alignWithWall(Back);
    driveTrain->driveDisplacement(5,targetAngle, 0.5);
    driveTrain->driveVelocity(0);

    ran = true;
  }
 lcd->display(String(mlxL.readAmbientTempC()));
 if(mlxL.readObjectTempC() - mlxL.readAmbientTempC() > 5){
    dispenser->dispenseDirection(DispenserDirection::left);
 }
 if(mlxR.readObjectTempC() - mlxR.readAmbientTempC() > 5){
    dispenser->dispenseDirection(DispenserDirection::right);
 }
 
 driveTrain->driveDisplacement(30,targetAngle, 0.5);
 int frontDistance, leftDistance, rightDistance;
 frontDistance = driveTrain->getDistanceFront();


  String distances;
  distances.concat("R= ");
  distances.concat(String(rightDistance));
  distances.concat("L= ");
  distances.concat(String(leftDistance));
  distances.concat("F= ");
  distances.concat(String(frontDistance));
  //lcd->display(distances);
  
  if(frontDistance < distanceThresh){
     lcd->display("Front wall!!!");
    driveTrain->turnToAngle(driveTrain->getYaw() + 90);
    rightDistance = driveTrain->getDistanceFront();
    driveTrain->turnToAngle(driveTrain->getYaw() - 179);
    leftDistance = driveTrain->getDistanceFront();

    if(leftDistance  > 14|| rightDistance  > 14){
      if(rightDistance > leftDistance){
          driveTrain->turnToAngle(targetAngle);
      }else{
          driveTrain->turnToAngle(targetAngle - 90);
      }
    }
      targetAngle = driveTrain->getYaw();
      driveTrain->alignWithWall(Back);

  }else{
     lcd->display("No wall!!!");
  }
//      while(driveTrain->getDistanceRight() < 15 || driveTrain->getDistanceLeft() < 15 ){
//        driveTrain->driveDisplacement(30, targetAngle, -.5);
//      }
//      if(driveTrain->getDistanceRight() > 15 ){
//        targetAngle = targetAngle + 90;
//     
//        if(targetAngle >=  180){
//          targetAngle -= 180; 
//        }
//        driveTrain->turnToAngle(targetAngle);
//        
//      }else{
//        targetAngle = targetAngle - 90;
//     
//        if(targetAngle <=  -180){
//          targetAngle += 180; 
//        }
//        driveTrain->turnToAngle(targetAngle);      
//      }
   delay(500);
}
void loop(){
     // servo.write(120);
   
  code();
//  int frontDistance, leftDistance, rightDistance;
//  frontDistance = driveTrain->getDistanceFront();
//  leftDistance = driveTrain->getDistanceLeft();
//  rightDistance = driveTrain->getDistanceRight();
//  Serial.print("F= ");
//  Serial.print(frontDistance);
//  Serial.print("\tL= ");
//  Serial.print(leftDistance);
//  Serial.print("\tR");
//  Serial.println(rightDistance);
}




#include "DriveTrain.h"
#include "Map.h"
#include "LCD.h"

DriveTrain* driveTrain;
//Map* maze;
LCD* lcd;

// Tile senseTile(){

//   TileConfig tileConfig; 

//   if(driveTrain->getPitch() > 20){ /// HACER GETPITCHHISTORY PARA SACAR PORMEDIO DE MULTIPLES LECTURAS
//   	tileConfig.isRamp = true;
//   }
  
  
//   if(driveTrain->getDistanceLeft() < 15){
//   	tileConfig.left = TileWall(true);
//   }

//   if(driveTrain->getDistanceRight() < 15){
//   	tileConfig.right = TileWall(true);
//   }
//   if(driveTrain->getDistanceFront() < 15){
//   	tileConfig.up = TileWall(true);
//   }

//   driveTrain->turnToAngle(90);
//   if(driveTrain->getDistanceRight() < 15){
//   	tileConfig.down = TileWall(true);
//   }

//   driveTrain->turnToAngle(0);

//   Tile tile(tileConfig);
//   return tile;
// }


void setup() {

  Serial.begin(9600);
  lcd = new LCD();

  lcd->display("Booting Up...");

  driveTrain = new DriveTrain();
  // TileConfig initialConfig; 

  // if(driveTrain->getDistanceLeft() < 15){
  // 	initialConfig.left = TileWall(true);
  // }

  // if(driveTrain->getDistanceRight() < 15){
  // 	initialConfig.right = TileWall(true);
  // }
  // if(driveTrain->getDistanceFront() < 15){
  // 	initialConfig.up = TileWall(true);
  // }

  // driveTrain->turnToAngle(90);
  // if(driveTrain->getDistanceRight() < 15){
  // 	initialConfig.down = TileWall(true);
  // }

  // driveTrain->turnToAngle(0);

  // Tile initialTile(initialConfig);
  // maze = new Map(initialTile);
}

void loop() {
  String str;
  // str.concat("R:");
  // str.concat(driveTrain->getDistanceRight());
  // str.concat(" F:");
  // str.concat(driveTrain->getDistanceFront());
  str.concat(" L:");
  str.concat(driveTrain->getDistanceLeft());
  lcd->display(str);
}




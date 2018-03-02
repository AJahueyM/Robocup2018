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

bool ran = false;
double speed = .5;
int wallThreshold = 15;
int angles[4];

enum Dir{
  shiftRight,
  shiftLeft
};

void shiftAngle(Dir dir);

void setup() {
  angles[0] = 0;
  angles[1] = 90;
  angles[2] = 180;
  angles[3] = -90;
  //Serial.begin(9600);
  //lcd = new LCD();

  //lcd->display("Booting Up...");

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
  if(!ran){
    driveTrain->alignWithWall(Back);
    driveTrain->driveDisplacement(6.27,angles[0], speed);
    ran = true;
  }

  if(driveTrain->getDistanceFront() > wallThreshold){
    driveTrain->driveDisplacement(30,angles[0],speed);
  }else if(driveTrain->getDistanceRight() > wallThreshold){
    driveTrain->turnToAngle(angles[1]);
    shiftAngle(shiftRight);
  }else if(driveTrain->getDistanceLeft() > wallThreshold){
    driveTrain->turnToAngle(angles[3]);
    shiftAngle(shiftLeft);
  }else{
    while(driveTrain->getDistanceRight() < wallThreshold && driveTrain->getDistanceLeft() < wallThreshold){
      driveTrain->driveDisplacement(30, angles[0], -speed);
    }
    if(driveTrain->getDistanceRight() > wallThreshold){
      driveTrain->turnToAngle(angles[1]);
      shiftAngle(shiftRight);

    }else if(driveTrain->getDistanceLeft() > wallThreshold){
      driveTrain->turnToAngle(angles[3]);
      shiftAngle(shiftLeft);

    }
  }
}

void shiftAngle(Dir dir){
  int first = angles[0]; 
  int second = angles[1];
  int third = angles[2];
  int fourth = angles[3];
  switch(dir){
    case shiftRight:
      angles[0] = second;
      angles[1] = third;
      angles[2] = fourth;
      angles[3] = first;
    break;

    case shiftLeft:
      angles[0] = fourth;
      angles[1] = first;
      angles[2] = second;
      angles[3] = third;
    break;
  };
}
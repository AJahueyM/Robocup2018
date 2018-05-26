#include "DriveTrain.h"
#include "Map.h"
#include "LCD.h"
#include "Utils.h"
#include "Gyro.h"

DriveTrain* driveTrain;
vector<Map*> maze;
LCD* lcd;
bool ranOnce = false;
double speed = .75;
int wallThreshold = 20;
int distanceMoveStart = 6;
int distanceMoveTiles = 30;
int angles[4];
int turnCounter = 0;
int mazeFloor = 0;
String str;
Direction currentRobotDirection = Up;
Coord currentRobotCoord;
void shiftAngle(Direction dir);
Tile getInitialTile();
Tile getCurrentTile();
Tile tile;

vector<int> hi;

void setup() {
  angles[0] = 0;
  angles[1] = 90;
  angles[2] = 180;
  angles[3] = -90;
  Serial.begin(9600);

  lcd = &LCD::getInstance();

  lcd->display("Booting Up...");
  driveTrain = new DriveTrain();
  lcd->display("Running initial setup...");
  // while(1){
  //   lcd->display(String(driveTrain->getTileColor()));
  // }
  driveTrain->alignWithWall(Back);

  driveTrain->driveDisplacement(distanceMoveStart,angles[0], 0.5);
  maze.push_back(new Map(getInitialTile()));

}
void updateTel (){
  String telemetry;
  //telemetry.concat(driveTrain->getTileColor());
  telemetry.concat("W:");
  telemetry.concat(maze[mazeFloor]->getWidth());
  telemetry.concat("H:");
  telemetry.concat(maze[mazeFloor]->getHeight());
  telemetry.concat(" ");
  telemetry.concat("X:");
  telemetry.concat(currentRobotCoord.getX());
  telemetry.concat(" ");
  telemetry.concat("Y:");
  telemetry.concat(currentRobotCoord.getY());
  telemetry.concat(" ");
  telemetry.concat(maze[mazeFloor]->getTileAt(currentRobotCoord).wallExists(Up));
  telemetry.concat(maze[mazeFloor]->getTileAt(currentRobotCoord).wallExists(Right));
  telemetry.concat(maze[mazeFloor]->getTileAt(currentRobotCoord).wallExists(Down));
  telemetry.concat(maze[mazeFloor]->getTileAt(currentRobotCoord).wallExists(Left));
  telemetry.concat(" ");
  // telemetry.concat(driveTrain->getDistanceFront());
  // telemetry.concat(" ");
  // telemetry.concat(driveTrain->getDistanceLeft());
  // telemetry.concat(" ");
  // telemetry.concat(driveTrain->getDistanceRight());
  switch(currentRobotDirection){
    case Up:
      telemetry.concat("U");
    break;

    case Right:
      telemetry.concat("R");
    break;

    case Down:
      telemetry.concat("D");
    break;

    case Left:
      telemetry.concat("L");
    break;
  }
  telemetry.concat(" ");
  telemetry.concat(maze[mazeFloor]->getTileAt(currentRobotCoord).wasVisited());
  telemetry.concat(" ");
  telemetry.concat(maze[mazeFloor]->getNonVisitedTiles());
  telemetry.concat(" ");
  telemetry.concat(maze[mazeFloor]->getTileAt(currentRobotCoord).getLeftKit());
  telemetry.concat(" ");
  telemetry.concat(mazeFloor);
  lcd->display(telemetry);
}

Tile getRobotTile(Direction dir){
  Coord tileCoord = currentRobotCoord;
  switch(dir){
    case Up:
      switch(currentRobotDirection){
            case Up:
              tileCoord.setY( tileCoord.getY() + 1);
              break;
            case Right:
              tileCoord.setX( tileCoord.getX() + 1);
              break;

            case Down:
              tileCoord.setY( tileCoord.getY() - 1);
              break;

            case Left:
              tileCoord.setX( tileCoord.getX() - 1);
              break;  
        }

    break;
    case Right:
      switch(currentRobotDirection){
            case Up:
              tileCoord.setX( tileCoord.getX() + 1);
              break;
            case Right:
              tileCoord.setY( tileCoord.getY() - 1);
              break;

            case Down:
              tileCoord.setX( tileCoord.getX() - 1);
              break;

            case Left:
              tileCoord.setY( tileCoord.getY() + 1);
              break;  
        }
    break;
    case Down:
      switch(currentRobotDirection){
            case Up:
              tileCoord.setY( tileCoord.getY() - 1);
              break;
            case Right:
              tileCoord.setX( tileCoord.getX() - 1);
              break;

            case Down:
              tileCoord.setY( tileCoord.getY() + 1);
              break;

            case Left:
              tileCoord.setX( tileCoord.getX() + 1);
              break;  
        }
    break;
    case Left:
      switch(currentRobotDirection){
            case Up:
              tileCoord.setX( tileCoord.getX() - 1);
              break;
            case Right:
              tileCoord.setY( tileCoord.getY() + 1);
              break;

            case Down:
              tileCoord.setX( tileCoord.getX() + 1);
              break;

            case Left:
              tileCoord.setY( tileCoord.getY() - 1);
              break;  
        }
    break;
  }

  return maze[mazeFloor]->getTileAt(tileCoord); 
}

void turnRobot(Direction dir){
  switch(dir){
    case Right:
      driveTrain->turnToAngle(angles[1]);
      shiftAngle(Right);
    break;

    case Left:
      driveTrain->turnToAngle(angles[3]);
      shiftAngle(Left);
    break;
    default:
    break;
  }
}
void driveForward(){
  driveTrain->driveDisplacement(distanceMoveTiles, angles[0], speed);
  if(driveTrain->wasLastDisplacementCompleted()){
    switch (currentRobotDirection) {
      case Up:
        currentRobotCoord.setY(currentRobotCoord.getY() + 1);
        break;
      case Right:
        currentRobotCoord.setX(currentRobotCoord.getX() + 1);
        break;

      case Down:
        currentRobotCoord.setY(currentRobotCoord.getY() - 1);
        break;

      case Left:
        currentRobotCoord.setX(currentRobotCoord.getX() - 1);
        break;
    }
    maze[mazeFloor]->setRobotCoord(currentRobotCoord);
  }else if(driveTrain->wasInterruptedColor()){
    Coord updateColorTile = currentRobotCoord;

    switch (currentRobotDirection) {
        case Up:
          updateColorTile.setY(updateColorTile.getY() + 1);
          break;
        case Right:
          updateColorTile.setX(updateColorTile.getX() + 1);
          break;

        case Down:
          updateColorTile.setY(updateColorTile.getY() - 1);
          break;

        case Left:
          updateColorTile.setX(updateColorTile.getX() - 1);
          break;
    }

    byte identity = B00000000, identity2 = B00000000;
    identity = identity | Tile::isBlackMask;
    identity2 = identity2 | Tile::maskVisited;

    Tile blackTile(identity, identity2);
    maze[mazeFloor]->setTileAt(updateColorTile, blackTile);

    driveTrain->driveDisplacement(10, angles[0], -.25, true);
  }
}

void loop() {

  if(ranOnce){
    if(!maze[mazeFloor]->getTileAt(currentRobotCoord).wasVisited()){
      tile = getCurrentTile();
      maze[mazeFloor]->setTileAt(currentRobotCoord, tile);
    }else{
      tile = maze[mazeFloor]->getTileAt(currentRobotCoord);
    }
  }else{
    tile = maze[mazeFloor]->getTileAt(currentRobotCoord);
  }
  ranOnce = true;
  driveTrain->setShouldDispense(!tile.getLeftKit());

  maze[mazeFloor]->expandMap();
  currentRobotCoord = maze[mazeFloor]->getRobotCoord();
  updateTel();
  Direction robotRight, robotLeft, robotUp, robotDown;
  switch (currentRobotDirection) {
      case Up:
        robotRight = Right;
        robotLeft = Left;
        robotUp = Up;
        robotDown = Down;
        break;
      case Right:
        robotRight = Down;
        robotLeft = Up;
        robotUp = Right;
        robotDown = Left;
        break;

      case Down:
        robotRight = Left;
        robotLeft = Right;
        robotUp = Down;
        robotDown = Up;

        break;

      case Left:
        robotRight = Up;
        robotLeft = Down;
        robotUp = Left;
        robotDown = Right;

        break;
  }
  if(tile.isRamp()){
    while(driveTrain->getPitch() > 10){
      lcd->display(String(driveTrain->getPitch()));
      driveTrain->driveStraight(angles[0], .5);
    }
    driveTrain->driveDisplacement(8, angles[0], .5);
    mazeFloor++;
    tile = getCurrentTile();
    maze.push_back(new Map(tile));
    maze[mazeFloor]->expandMap();
    currentRobotCoord = maze[mazeFloor]->getRobotCoord();
  }


  if(turnCounter >= 5 && tile.wallExists(Down)){
    double lastAngle = angles[0];
    driveTrain->turnToAngle(0);
    driveTrain->alignWithWall(Back);
    driveTrain->driveDisplacement(distanceMoveStart,0, 0.5);
    driveTrain->turnToAngle(lastAngle);
    turnCounter = 0;
  }
  if(tile.wallExists(robotUp)){
    while(driveTrain->getDistanceFront() != 9){
      if(driveTrain->getDistanceFront() > 9)
        driveTrain->driveStraight(angles[0], .25);
      else if(driveTrain->getDistanceFront() < 9)
        driveTrain->driveStraight(angles[0], -.25);
    }
    driveTrain->turn(0);
  }

  if(!tile.wallExists(robotUp) && getRobotTile(Up).getColor() != Black){
      // if((getRobotTile(Right).wasVisited() && !tile.wallExists(robotRight)) || (getRobotTile(Left).wasVisited() && !tile.wallExists(robotLeft))){
      //   int rand = random(1);
      //   if(rand == 1){
      //     if(!tile.wallExists(robotRight)){
      //       turnRobot(Right);
      //     }else if(!tile.wallExists(robotLeft)){
      //       turnRobot(Left);
      //     }
      //   }
      // }
    if(!getRobotTile(Up).wasVisited()){

    }else if((!tile.wallExists(robotRight) && getRobotTile(Right).wasVisited()) || (!tile.wallExists(robotLeft) && getRobotTile(Left).wasVisited())) {
      int rand = random(100);
      if(rand > 49){
        if(!tile.wallExists(robotRight)){
          turnRobot(Right);
        }else{
          turnRobot(Left);
        }
      }
      
    }else{
      if(!getRobotTile(Right).wasVisited() || !getRobotTile(Left).wasVisited()){
        if(!getRobotTile(Right).wasVisited() && !tile.wallExists(robotRight)){
          turnRobot(Right);
        }else if(!tile.wallExists(robotLeft)){
          turnRobot(Left);
        }
      }else{
        if(!tile.wallExists(robotRight) && getRobotTile(Right).getColor() != Black){
          turnRobot(Right);
        }else if(!tile.wallExists(robotLeft) && getRobotTile(Left).getColor() != Black){
          turnRobot(Left);
        }
      }
    }
    driveForward();
  }else if(!tile.wallExists(robotRight) || !tile.wallExists(robotLeft)){

    if(!getRobotTile(Right).wasVisited() || !getRobotTile(Left).wasVisited()){

      if(!getRobotTile(Right).wasVisited() && !tile.wallExists(robotRight)){
        turnRobot(Right);
      }else if(!tile.wallExists(robotLeft)){
        turnRobot(Left);
      }

    }else{
      int rand = random(100);
      if(rand > 49){
        if(!tile.wallExists(robotRight)){
          turnRobot(Right);
        }else{
          turnRobot(Left);
        }
      }else{
        if(!tile.wallExists(robotLeft)){
          turnRobot(Left);
        }else{
          turnRobot(Right);
        }
      }
     
    }
    driveForward();
  }else{

    while(driveTrain->getDistanceFront() < wallThreshold){
      turnRobot(Right);
    }
    driveForward();
  }

  if(driveTrain->leftKitLastMovement()){
    Tile oldTile = maze[mazeFloor]->getTileAt(currentRobotCoord);
    oldTile.setLeftKit(true);
    maze[mazeFloor]->setTileAt(currentRobotCoord, oldTile);
  }
  driveTrain->setLeftKit(false);
  updateTel();
}

void shiftAngle(Direction dir){
  turnCounter++;
  int first = angles[0]; 
  int second = angles[1];
  int third = angles[2];
  int fourth = angles[3];
  switch(dir){
    case Right:
      angles[0] = second;
      angles[1] = third;
      angles[2] = fourth;
      angles[3] = first;
    break;

    case Left:
      angles[0] = fourth;
      angles[1] = first;
      angles[2] = second;
      angles[3] = third;
    break;
  };

  first = angles[0]; 
  second = angles[1];
  third = angles[2];
  fourth = angles[3];

  if(first == 0)
      currentRobotDirection = Up;
  else if(second == 0)
      currentRobotDirection = Left;
  else if(third == 0)
      currentRobotDirection = Down;
  else if(fourth == 0)
      currentRobotDirection = Right;
}

Tile getInitialTile(){
  byte result = B00000000, result2 = B00000000;

  if(driveTrain->getDistanceFront() < wallThreshold){
    Serial.print("F");
    result = result | Tile::wallUpMask;
  }
  
  if(driveTrain->getDistanceLeft() < wallThreshold){
      Serial.print("L");

    result = result | Tile::wallLeftMask;
  }

  if(driveTrain->getDistanceRight() < wallThreshold){
        Serial.print("R");

    result = result | Tile::wallRightMask;
  }
  driveTrain->turnToAngle(90);

  if(driveTrain->getDistanceRight() < wallThreshold){
        Serial.print("B");

    result = result | Tile::wallDownMask;
  }
          Serial.println();

  driveTrain->turnToAngle(0);

  Color sensedColor = driveTrain->getTileColor();
  switch (sensedColor) {
      case White:
        break;

      case Black:
        result = result | Tile::isBlackMask;
        break;

      case Silver:
        result = result | Tile::isCheckpointMask;
        break;

      default:
        break;
  }
  result2 = result2 | Tile::maskVisited;
  return Tile(result, result2);
}

Tile getCurrentTile(){
  byte result = B00000000, result2 = B00000000;
  
  if(driveTrain->getDistanceFront() < wallThreshold){
    //Serial.print("F");
    switch(currentRobotDirection){
      case Up:
        result = result | Tile::wallUpMask;
        break;

      case Down:
        result = result | Tile::wallDownMask;
        break;

      case Right:
        result = result | Tile::wallRightMask;

        break;

      case Left:
        result = result | Tile::wallLeftMask;
        break;

      default:
        break;
    }
  }
  
  if(driveTrain->getDistanceLeft() < wallThreshold){
    //Serial.print("L");

    switch(currentRobotDirection){
      case Up:
        result = result | Tile::wallLeftMask;
        break;

      case Down:
        result = result | Tile::wallRightMask;
        break;

      case Right:
        result = result | Tile::wallUpMask;
        break;

      case Left:
        result = result | Tile::wallDownMask;
        break;

      default:
        break;
    }
  }

  if(driveTrain->getDistanceRight() < wallThreshold){
    //Serial.print("R");

    switch(currentRobotDirection){
      case Up:
        result = result | Tile::wallRightMask;
        break;

      case Down:
        result = result | Tile::wallLeftMask;
        break;

      case Right:
        result = result | Tile::wallDownMask;

        break;

      case Left:
        result = result | Tile::wallUpMask;
        break;

      default:
        break;
    }
  }
  Color sensedColor = driveTrain->getTileColor();
  switch (sensedColor) {
      case White:
        break;

      case Black:
        result = result | Tile::isBlackMask;
        break;

      case Silver:
        result = result | Tile::isCheckpointMask;
        break;

      default:
        break;
  }
   // Serial.println();

  if(driveTrain->getPitch() > 0 ){
    if(driveTrain->getPitch() > 20)
    result = result | Tile::isRampMask;
  }else if(driveTrain->getPitch() > 5 && driveTrain->getPitch() < 20){
    result2 = result2 | Tile::hasBumpMask; // 
  }
  result2 = result2 | Tile::maskVisited;
  return Tile(result, result2);
}
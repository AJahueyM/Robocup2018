#include "Cerebrum.h"
#include <MemoryFree.h>
#include "Tile.h"
#include "AStar.h"
#include "Map.h"
DriveTrain* driveTrain;
Cerebrum* cerebrum;

vector<vector<Tile>> tileMap;

uint8_t rows = 5, cols = 4;

void setup() {
  Serial.begin(9600);
  driveTrain = &DriveTrain::getInstance(); 
  cerebrum = &Cerebrum::getInstance(*driveTrain);
  //cerebrum->start();

  for(int y = 0; y < rows; ++y){
    vector<Tile> row;
    tileMap.push_back(row);
    for(int x = 0; x < cols; ++x){
      tileMap[y].push_back(Tile(x, y));
    }
  }
  
  tileMap[1][0].setWall(Right, true);
  tileMap[2][1].setWall(Up, true);
  tileMap[2][1].setWall(Right, true);

  tileMap[1][2].setWall(Right, true);
  tileMap[2][2].setWall(Right,true);
  tileMap[3][2].setWall(Right, true);

  tileMap[2][1].setBumpLevel(Max);
  tileMap[1][3].setBumpLevel(Max);
  tileMap[2][2].setBumpLevel(Max);

  tileMap[2][0].setBumpLevel(Small);
  tileMap[4][0].setBumpLevel(Medium);
  tileMap[4][1].setBumpLevel(Medium);
  tileMap[4][2].setBumpLevel(Medium);
  tileMap[3][3].setBumpLevel(Small);

  cout << tileMap[2][0].getCost() << " " << tileMap[4][0].getCost() << " " << tileMap[2][1].getCost() << endl;
  Map tMap(tileMap);
  
  Coord start(0,0);
  Coord end(3,2);
  Path path = AStar::getPath(start, end, tMap.getTileMap());
  cout << "StartX= " <<  start.getX() << " StartY= " << start.getY() << " EndX= " << end.getX() << " EndY= " << end.getY() << endl;
  for(int i = 0; i < path.getLength(); ++i){
   cout << "X= " << path.getCoordAt(i).getX() << " Y= " << path.getCoordAt(i).getY() << endl;
  }

  cout << freeMemory() << endl;
}

void loop() {
  //cerebrum->update();
}

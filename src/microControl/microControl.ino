// #include "Cerebrum.h"
#include <MemoryFree.h>
#include "Sharp.h"
#include "Absis.h"
#include <ArduinoSTL.h>
using namespace std;
#include "Tile.h"
#include "AStar.h"
#include "Map.h"
// DriveTrain* driveTrain;
// Cerebrum* cerebrum;


const uint8_t rows = 15, cols = 15;
//Tile tileMap [rows] [cols];
//Sharp* sharp;
void setup() {
  Serial.begin(115200);
 
  Absis<Absis<Tile>> tileMap;
  for(int y = 0; y < rows; ++y){
    tileMap.emplace_back(Absis<Tile>());
    for(int x = 0; x < cols; ++x){
      tileMap[y].emplace_back(Tile(x, y));
    }
  }
  tileMap[0][0].visited(true);

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

  //cout <<  (int) tileMap[2][0].getCost() << " " <<  (int) tileMap[4][0].getCost() << " " <<  (int) tileMap[2][1].getCost() << endl;
  Map tMap(tileMap);
  
  Coord start(0,0);
  Coord end(1,0);
 Path path = AStar::getPath(start, end, tMap.getTileMap());
  cout << "StartX= " <<  (int)start.getX() << " StartY= " << (int)start.getY() << " EndX= " << (int)end.getX() << " EndY= " << (int)end.getY() << endl;
  for(int i = 0; i < path.getLength(); ++i){
   cout << "X= " << (int) path.getCoordAt(i).getX() << " Y= " << (int) path.getCoordAt(i).getY() << endl;
  }

  cout << freeMemory() << endl;

}

void loop() {
  //cout << 0 << endl;
  //cerebrum->update();
}

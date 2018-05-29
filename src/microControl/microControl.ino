//#include "Cerebrum.h"
#include <MemoryFree.h>
#include "Tile.h"
#include "AStar.h"
#include "Map.h"
//DriveTrain* driveTrain;
//Cerebrum* cerebrum;

vector<vector<Tile>> tileMap;

uint8_t rows = 7, cols = 5;

void setup() {
  Serial.begin(9600);
  //driveTrain = &DriveTrain::getInstance(); 
  //cerebrum = &Cerebrum::getInstance(*driveTrain);
  //cerebrum->start();
  for(int y = 0; y < rows; ++y){
    vector<Tile> row;
    tileMap.push_back(row);
    for(int x = 0; x < cols; ++x){
      tileMap[y].push_back(Tile(x, y));
    }
  }
  
  Map tMap(tileMap);
  Path path = AStar::getPath(Coord(0,0), Coord(2,0), tMap.getTileMap());

  // for(int i = 0; i < path.getLenght(); ++i){
  //   cout << "X= " << path.getCoordAt(i).getX() << " Y= " << path.getCoordAt(i).getY() << endl;
  // }

  cout << freeMemory() << endl;
}

void loop() {
  //cerebrum->update();
}

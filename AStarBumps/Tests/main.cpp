#include <vector>
#include "Map.h"
#include "AStar.h"
#include <iostream>
using namespace std;

uint8_t mazeFloor = 0;
Absis<Map*> maze;

enum StopReason {
	Completed,
	NoCandidates,
	RampReached
};

struct NavigationResult {
	Coord endCoord;
	StopReason endReason;
};


Path getPathLowerCost(Coord start, vector<Coord> targets, Map* map){
		uint8_t minorCost = 254;
		Path bestPath;
		for(Coord end  : targets){
			Path candidatePath = AStar::getPath(start, end, map->getTileMap());
			uint8_t cost = candidatePath.getCost();
			cout << "CANDIDATE COST= " << (int) cost <<"\tx: " <<(int)  end.getX() << "\ty: " <<(int)  end.getY() << endl;
			if(!candidatePath.getValid()){
				map->getTileAt(end).visited(true);
			}
			if( cost < minorCost && candidatePath.getValid()){
				minorCost = candidatePath.getCost();
				bestPath = candidatePath;
			}
		}
		return bestPath;
}

bool roundCompleted(Absis<Map*> maps){
	for(int i = 0; i < maps.size(); ++i){
		Map* map = maps[i];
		if(!map->wasCompleted()){
			return false;
		}
	}
	return true;
}
NavigationResult navigateLevel(Map* mapCurrent, Coord startCoord){ //Returns last coordinate reached
	NavigationResult data;
	vector<Coord> candidates = mapCurrent->getCandidates();
	Coord start = mapCurrent->getTileAt(startCoord);
	Coord target;
	
	while(candidates.size() > 0){
		vector<Coord> targets;
		

		targets = getClosestFrom(candidates, start);
		
		Path bestPath = getPathLowerCost(start, targets, mapCurrent);
		

		if(bestPath.getValid()){
			target = bestPath.getCoordAt(bestPath.getLength()-1);
			cout << "StartX: " << (int) start.getX() << " StartY: " << (int)start.getY() << " Floor: " << (int) mapCurrent->getLevelNum() <<  endl;
			cout << "EndX: " << (int)target.getX() << " EndY: " <<(int) target.getY() << endl;
			bestPath.print();
			/* TODO= SCAN TILE AND UPDATE MAP.TEST ONCE WE HAVE ROBOT
				followPath(bestPath);
				getCurrentTile();
				UpdateMap;

			*/

			if(mapCurrent->getTileAt(target).isRamp()){

				data.endCoord = target;
				data.endReason = RampReached;
				return data;
			}

			#ifdef ARDUINO
			cout << "free= " << freeMemory() << endl;
			#endif
			start = target;
		}
		mapCurrent->getTileAt(target).visited(true);		/// COMENT THIS WHEN TESTING WITH ROBOT

		candidates = mapCurrent->getCandidates();
		char c;
		cin >> c;
	}
	data.endCoord = target;

	if(mapCurrent->wasCompleted()){
		data.endReason = Completed;
	}else{
		Absis<Ramp> ramps = mapCurrent->getRamps();
		vector<Coord> targetCoords;
		for(int i = 0; i < ramps.size(); ++i){
			Ramp ramp = ramps[i];
			if(!ramp.getUsed())
				targetCoords.emplace_back(ramp.getOrigin());
		}

		Path pathToClosestRamp = getPathLowerCost(data.endCoord, targetCoords, mapCurrent);

		/* GO TO RAMP
		followPath(pathToClosestRamp);
		*/
		pathToClosestRamp.print();
		data.endReason = RampReached;
		data.endCoord = pathToClosestRamp.getCoordAt(pathToClosestRamp.getLength() -1);
	}
	return data;
}

void start(){
	Absis<Absis<Tile>> tileMap, tileMap2, tileMap3;
	const uint8_t rows = 7, cols = 5, rows1 = 3, cols1 = 3;

	for(int y = 0; y < rows; ++y){
		Absis<Tile> row;
		tileMap.emplace_back(row);

		for(int x = 0; x < cols; ++x){
			tileMap[y].emplace_back(Tile(x, y));
		}
	}

	for(int y = 0; y < rows1; ++y){
		Absis<Tile> row;
		tileMap2.emplace_back(row);

		for(int x = 0; x < cols1; ++x){
			tileMap2[y].emplace_back(Tile(x, y));
		}
	}	

	for(int y = 0; y < rows1; ++y){
		Absis<Tile> row;
		tileMap3.emplace_back(row);

		for(int x = 0; x < cols1; ++x){
			tileMap3[y].emplace_back(Tile(x, y));
		}
	}	

	tileMap[0][0].visited(true);

	tileMap[1][0].setWall(Down, true);
	tileMap[3][0].setWall(Down, true);
	tileMap[0][1].setWall(Down, true);
	tileMap[2][1].setWall(Down, true);
	tileMap[3][1].setWall(Left, true);
	tileMap[4][1].setWall(Left, true);
	tileMap[4][1].setWall(Down, true);
	tileMap[5][1].setWall(Left, true);
	tileMap[5][1].setWall(Down, true);
	tileMap[0][2].setWall(Down, true);
	tileMap[4][2].setWall(Down, true);
	tileMap[0][3].setWall(Down, true);
	tileMap[3][3].setWall(Down, true);
	//tileMap[3][3].setWall(Right, true);
	tileMap[1][4].setWall(Left, true);
	tileMap[2][4].setWall(Down, true);
	tileMap[4][4].setWall(Left, true);
	tileMap[5][4].setWall(Left, true);
	tileMap[6][4].setWall(Left, true);

	tileMap[2][0].setBumpLevel(Small);
	tileMap[5][0].setBumpLevel(Small);
	tileMap[3][1].setBumpLevel(Medium);
	tileMap[4][1].setBumpLevel(Max);
	tileMap[5][1].setBumpLevel(Max);
	tileMap[0][2].setBumpLevel(Small);
	tileMap[2][2].setBumpLevel(Small);
	tileMap[5][3].setBumpLevel(Medium);
	tileMap[2][4].setBumpLevel(Small);
	tileMap[5][4].setBumpLevel(Small);


  //cout <<  (int) tileMap[2][0].getCost() << " " <<  (int) tileMap[4][0].getCost() << " " <<  (int) tileMap[2][1].getCost() << endl;
  	//Map tMap(tileMap);

	Map* map1 = new Map(tileMap, 0);
	Map* map2 = new Map(tileMap2, 1);
	Map* map3 = new Map(tileMap3, 2);

	Map::createRamp(map1, &map1->getTileAt(Coord(4,0)), map2, &map2->getTileAt(Coord(0,0)));
	Map::createRamp(map1, &map1->getTileAt(Coord(0,4)), map3, &map3->getTileAt(Coord(2,0)));

	maze.emplace_back(map1);
	maze.emplace_back(map2);
	maze.emplace_back(map3);
}

void run(){

	Tile* startTile = &maze[mazeFloor]->getTileMap()[0][0];
	NavigationResult navigationResult;
	Coord currentCoord(0,0);

	while(!roundCompleted(maze)){
		//cout << "LEVEL--------------" << (int) maze[mazeFloor]->getLevelNum() << endl;
		navigationResult = navigateLevel(maze[mazeFloor], currentCoord);
		if(navigationResult.endReason == RampReached){
			Ramp* ramp = maze[mazeFloor]->getRampAt(navigationResult.endCoord);
			mazeFloor = ramp->getTargetFloor();
			ramp->setUsed(true);
			cout << "RAMP-----CHANGE LEVEL------ " << (int) mazeFloor<< endl;

			currentCoord = ramp->getEnd();
			maze[mazeFloor]->getTileAt(currentCoord).visited(true);
			/*REMEMBER TO UPDATE CURRENT ROBOT COORDINATE*/
		}
	}

	while(mazeFloor != 0){
		Coord rampTarget;
		Map* mapCurrent = maze[mazeFloor];
		Absis<Ramp> ramps = mapCurrent->getRamps();
		
		int lowerLevel = maze.size() -1;
		Ramp bestRamp;

		for(int i = 0; i < ramps.size(); ++i){
			Ramp ramp = ramps[i];
			if(ramp.getTargetFloor() < lowerLevel){
				bestRamp = ramp;
				lowerLevel = ramp.getTargetFloor();
			}
		}

		Path pathToRamp = AStar::getPath(currentCoord, bestRamp.getOrigin(), mapCurrent->getTileMap());
		pathToRamp.print();
		/*RUN PATH*/
		mazeFloor = bestRamp.getTargetFloor();
		currentCoord = bestRamp.getEnd();
		/*UPDATE ROBOT COORD*/
	}

	Coord returnTo = *startTile;
	Path returnPath = AStar::getPath(currentCoord, returnTo, maze[mazeFloor]->getTileMap());
	cout << "REGRESANDO" << endl;
	returnPath.print();
	/*RUN PATH AND FINISH*/
}

int main(){
	//resetGyro();
	// maze[mazeFloor] = (new Map(getInitialTile()));
	start();

	run();

}
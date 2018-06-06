#include "Cerebrum.h"

Cerebrum::Cerebrum(DriveTrain& driveTrain) : driveTrain(driveTrain), lcd(LCD::getInstance()){
	angles[0] = 0;
	angles[1] = 90;
	angles[2] = 180;
	angles[3] = -90;

	button1 = new Button(30);
	button2 = new Button(28);

	lcd.display(String("Robot booted up"));
}

Path Cerebrum::getPathLowerCost(Coord start, vector<Coord> targets, Map* map){
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

bool Cerebrum::roundCompleted(Absis<Map*> maps){
	for(int i = 0; i < maps.size(); ++i){
		Map* map = maps[i];
		if(!map->wasCompleted()){
			return false;
		}
	}
	return true;
}
NavigationResult Cerebrum::navigateLevel(Map* mapCurrent, Coord startCoord){ //Returns last coordinate reached
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
			*/
			currentRobotCoord = target;

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
				targetCoords.push_back(ramp.getOrigin());
		}

		Path pathToClosestRamp = getPathLowerCost(data.endCoord, targetCoords, mapCurrent);

		/* GO TO RAMP
			followPath(bestPath);
		*/
		pathToClosestRamp.print();
		data.endReason = RampReached;
		data.endCoord = pathToClosestRamp.getCoordAt(pathToClosestRamp.getLength() -1);
	}
	return data;
}

vector<Coord> Cerebrum::getCandidates(){
	vector <Coord> vec;
	Map* tileMap = this->maze[mazeFloor];
	Absis<Absis<Tile>> &maze = tileMap->getTileMap();
	for (int y = 0; y < maze.size(); ++y) {
		for (int x = 0; x < maze[0].size(); ++x) {
			if (maze[y][x].wasVisited()) {

				//cout << "LA TILE VISITADA ES (" << x << "," << y << ")" << endl;
				vector <Tile*> neighbors;
				for(int i = 0; i < maze[y][x].getCurrentNeighbors(); ++i){
					neighbors.push_back( (Tile*) maze[y][x].getNeighbors(i));
				}

				//cout << "SUS VECINOS NO VISITADOS SON : ";

				for (int i = neighbors.size() - 1; i >= 0; --i) {
					if (!neighbors[i]->wasVisited()) {
						Coord elem = (Coord) *neighbors[i];
						if (countsOnVector(vec, elem) == 0) {
							vec.push_back(elem);
							//cout << "(" << neighbors[i]->getX() << "," << neighbors[i]->getY() << ")  ";
						}
					}
				}

				//cout << endl;
			}
		}
	}
	return vec;	
}

void Cerebrum::start(){
	Absis<Absis<Tile>> tileMap, tileMap2, tileMap3;
	const uint8_t rows = 7, cols = 5, rows1 = 3, cols1 = 3;

	for(int y = 0; y < rows; ++y){
		Absis<Tile> row;
		tileMap.push_back(row);

		for(int x = 0; x < cols; ++x){
			tileMap[y].push_back(Tile(x, y));
		}
	}

	for(int y = 0; y < rows1; ++y){
		Absis<Tile> row;
		tileMap2.push_back(row);

		for(int x = 0; x < cols1; ++x){
			tileMap2[y].push_back(Tile(x, y));
		}
	}	

	for(int y = 0; y < rows1; ++y){
		Absis<Tile> row;
		tileMap3.push_back(row);

		for(int x = 0; x < cols1; ++x){
			tileMap3[y].push_back(Tile(x, y));
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

	maze.push_back(map1);
	maze.push_back(map2);
	maze.push_back(map3);

}


void Cerebrum::run(){

	Tile* startTile = &maze[mazeFloor]->getTileMap()[0][0];
	NavigationResult navigationResult;
	Coord currentCoord(0,0);

	while(!roundCompleted(maze)){
		//cout << "LEVEL--------------" << (int) maze[mazeFloor]->getLevelNum() << endl;
		navigationResult = navigateLevel(maze[mazeFloor], currentCoord);
		if(navigationResult.endReason == RampReached){

			/*
			ADVANCE UNTIL RAMP IS REACHED
			getCurrentTile

			*/
			Ramp* ramp = maze[mazeFloor]->getRampAt(navigationResult.endCoord);
			mazeFloor = ramp->getTargetFloor();
			ramp->setUsed(true);
			cout << "RAMP-----CHANGE LEVEL------ " << (int) mazeFloor<< endl;

			currentCoord = ramp->getEnd();
			maze[mazeFloor]->getTileAt(currentCoord).visited(true);
			currentRobotCoord = currentCoord;
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
		/*RUN PATH
		followPath(pathToRamp);
		*/
		mazeFloor = bestRamp.getTargetFloor();
		currentCoord = bestRamp.getEnd();
		currentRobotCoord = currentCoord;
		/*UPDATE ROBOT COORD
		
		*/
	}

	Coord returnTo = *startTile;
	Path returnPath = AStar::getPath(currentCoord, returnTo, maze[mazeFloor]->getTileMap());
	cout << "REGRESANDO" << endl;
	returnPath.print();
	/*RUN PATH AND FINISH
	followPath(returnTo);
	*/

}

void Cerebrum::followPath(Path& path){
	Tile currentTile = maze[mazeFloor]->getTileAt(currentRobotCoord);
	driveTrain.setShouldDispense(!currentTile.getLeftKit());
	for(Coord coord : path.getPath()){
		int deltaX = coord.getX() - currentRobotCoord.getX();
		int deltaY = coord.getY() - currentRobotCoord.getY();

		if(deltaX > 0){
			if(currentRobotDirection == Up){
				turnRobot(Right);
			}else if(currentRobotDirection == Down){
				turnRobot(Left);
			}else if(currentRobotDirection == Left){
				turnRobot(Right);
				turnRobot(Right);
			}
			driveForward();
		}else if(deltaX < 0){
			if(currentRobotDirection == Right){
				turnRobot(Left);
				turnRobot(Left);
			}else if(currentRobotDirection == Up){
				turnRobot(Left);
			}else if(currentRobotDirection == Down){
				turnRobot(Right);
			}
			driveForward();
			
		}else if(deltaY > 0){
			if(currentRobotDirection == Right){
				turnRobot(Right);
			}else if(currentRobotDirection == Up){
				turnRobot(Left);
				turnRobot(Left);
			}else if(currentRobotDirection == Left){
				turnRobot(Left);
			}
			driveForward();
		}else if(deltaY < 0){
			if(currentRobotDirection == Right){
				turnRobot(Left);
			}else if(currentRobotDirection == Left){
				turnRobot(Right);
			}else if(currentRobotDirection == Down){
				turnRobot(Right);
				turnRobot(Right);
			}
			driveForward();
		}
	}
	while(currentTile.wallExists(robotUp) &&  driveTrain.getDistanceFront() != driveTrain.getDesiredWallDistance()){
		if(driveTrain.getDistanceFront() > driveTrain.getDesiredWallDistance()){
			driveTrain.driveStraight(angles[0], movementSpeed);
		}else{
			driveTrain.driveStraight(angles[0], -movementSpeed);
		}
	}
}

void Cerebrum::driveForward(){
	driveTrain.driveDisplacement(distanceMoveTiles, angles[0], movementSpeed);
	if(driveTrain.wasLastDisplacementCompleted()){
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
	}else if(driveTrain.wasInterruptedColor()){
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
		identity = identity | isBlackMask;
		identity = identity | wallUpMask;
		identity = identity | wallRightMask;
		identity = identity | wallDownMask;
		identity = identity | wallLeftMask;

		identity2 = identity2 | maskVisited;

		Tile blackTile(identity, identity2);
		maze[mazeFloor]->setTileAt(updateColorTile, blackTile);

		driveTrain.driveDisplacement(10, angles[0], movementSpeed * -1, true);
	}
}

void Cerebrum::turnRobot(Direction dir){
	switch(dir){
		case Right:
			turnCounter++;
			driveTrain.turnToAngle(angles[1]);
			shiftAngle(Right);
		break;

		case Left:
			turnCounter++;
			driveTrain.turnToAngle(angles[3]);
			shiftAngle(Left);
		break;
		default:
		break;
	}
	updateRobotOrientations();
}

void Cerebrum::shiftAngle(Direction dir){
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

Tile Cerebrum::getInitialTile(){
	byte result = B00000000, result2 = B00000000;

	if(driveTrain.getDistanceFront() < wallThreshold){
		Serial.print("F");
		result = result | wallUpMask;
	}

	if(driveTrain.getDistanceLeftFront() < wallThreshold){
		Serial.print("L");
		result = result | wallLeftMask;
	}

	if(driveTrain.getDistanceRightFront() < wallThreshold){
		Serial.print("R");
		result = result | wallRightMask;
	}


	if(driveTrain.getDistanceBack() < wallThreshold){
		Serial.print("B");

		result = result | wallDownMask;
	}
	Serial.println();

	driveTrain.turnToAngle(0);

	Color sensedColor = driveTrain.getTileColor();
	switch (sensedColor) {
		case White:
		break;

		case Black:
			result = result | isBlackMask;
		break;

		case Silver:
			result = result | isCheckpointMask;
		break;

		default:
		break;
	}
	result2 = result2 | maskVisited;
	return Tile(result, result2);
}

Tile Cerebrum::getCurrentTile(){
	byte result = B00000000, result2 = B00000000;
	if(driveTrain.getDistanceFront() < wallThreshold){
		//Serial.print("F");
		switch(currentRobotDirection){
			case Up:
				result = result | wallUpMask;
			break;

			case Down:
				result = result | wallDownMask;
			break;

			case Right:
				result = result | wallRightMask;

			break;

			case Left:
				result = result | wallLeftMask;
			break;

			default:
			break;
		}
	}

	if(driveTrain.getDistanceLeftFront() < wallThreshold){
		//Serial.print("L");

		switch(currentRobotDirection){
			case Up:
				result = result | wallLeftMask;
			break;

			case Down:
				result = result | wallRightMask;
			break;

			case Right:
				result = result | wallUpMask;
			break;

			case Left:
				result = result | wallDownMask;
			break;

			default:
			break;
		}
	}

	if(driveTrain.getDistanceRightFront() < wallThreshold){
		//Serial.print("R");

		switch(currentRobotDirection){
			case Up:
				result = result | wallRightMask;
			break;

			case Down:
				result = result | wallLeftMask;
			break;

			case Right:
				result = result | wallDownMask;

			break;

			case Left:
				result = result | wallUpMask;
			break;

			default:
			break;
		}
	}
	Color sensedColor = driveTrain.getTileColor();
	switch (sensedColor) {
		case White:
		break;

		case Black:
			result = result | isBlackMask;
		break;

		case Silver:
			result = result | isCheckpointMask;
		break;

		default:
		break;
	}
	// Serial.println();

	if(driveTrain.getPitch() > 0 ){
		if(driveTrain.getPitch() > 20)
			result = result | isRampMask;
	}
	result2 = result2 | maskVisited;
	Tile tile(result, result2);
	Absis<int> pitchHistory = driveTrain.getPitchHistory();
	int lowestAngle = 300, highestAngle = -300;

	for(int i = 0; i < pitchHistory.size(); ++i){
		int angle = pitchHistory[i];
		if(angle < lowestAngle){
			lowestAngle = angle;
		}
		if(angle > highestAngle){
			lowestAngle = angle;
		}
	}
	int angleDiff = abs(highestAngle - lowestAngle);
	tile.setBumpLevel(Flat);
	if(angleDiff > lowBumpAngleDiff){
		tile.setBumpLevel(Small);
	}
	if(angleDiff > mediumAngleDiff){
		tile.setBumpLevel(Medium);
	}
	if(angleDiff > maxAngleDiff){
		tile.setBumpLevel(Max);
	}

	if(driveTrain.leftKitLastMovement()){
		tile.setLeftKit(true);
	}else{
		driveTrain.setLeftKit(false);
	}
	return tile;
}

Tile Cerebrum::getRobotTile(Direction dir){
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

void Cerebrum::resetGyro(){
	const double distanceMoveStart = 6, speedMoveStart = .5;
	driveTrain.turnToAngle(0);
	driveTrain.alignWithWall(Back);
	driveTrain.resetAll();
	driveTrain.driveDisplacement(distanceMoveStart,angles[0], speedMoveStart);
}

void Cerebrum::updateRobotOrientations(){
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
}
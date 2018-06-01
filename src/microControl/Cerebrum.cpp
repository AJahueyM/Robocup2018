#include "Cerebrum.h"

Cerebrum::Cerebrum(DriveTrain& driveTrain) : driveTrain(driveTrain), lcd(LCD::getInstance()){
	angles[0] = 0;
	angles[1] = 90;
	angles[2] = 180;
	angles[3] = -90;
	

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
					neighbors.push_back(maze[y][x].getNeighbors(i));
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
	//resetGyro();
	// maze[mazeFloor] = (new Map(getInitialTile()));

	Absis<Absis<Tile>> tileMap;
	const uint8_t rows = 7, cols = 5;

	for(int y = 0; y < rows; ++y){
		Absis<Tile> row;
		tileMap.emplace_back(row);

		for(int x = 0; x < cols; ++x){
			tileMap[y].emplace_back(Tile(x, y));
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
	tileMap[3][3].setWall(Right, true);
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
	maze[mazeFloor] = new Map(tileMap);

}


void Cerebrum::run(){
	vector<Coord> candidates = getCandidates();
	Map* mazeCurrent = maze[mazeFloor];
	Tile* startTile = &mazeCurrent->getTileMap()[0][0];
	Coord start = mazeCurrent->getTileMap()[0][0];

	Coord target;
	while(candidates.size() > 0){

		vector<Coord> targets = getClosestFrom(candidates, start);
		Path bestPath;
		uint8_t minorCost = 254;

		for(Coord end  : targets){
			Path candidatePath = AStar::getPath(start, end, mazeCurrent->getTileMap());
			uint8_t cost = candidatePath.getCost();
			cout << "CANDIDATE COST= " << (int) cost <<"\tx: " <<(int)  end.getX() << "\ty: " <<(int)  end.getY() << endl;
			if(!candidatePath.getValid()){
				mazeCurrent->getTileMap()[end.getY()][end.getX()].visited(true);
			}
			if( cost < minorCost && candidatePath.getValid()){
				minorCost = candidatePath.getCost();
				bestPath = candidatePath;
				target = end;
			}
		}

		if(bestPath.getValid()){
			cout << "StartX: " << (int) start.getX() << " StartY: " << (int)start.getY() << endl;
			cout << "EndX: " << (int)target.getX() << " EndY: " <<(int) target.getY() << endl;
			bestPath.print();
			cout << "free= " << freeMemory() << endl;

		/*
			// FOLLOW PATH
			//followPath(bestPath);
			///READ TILE AND SET ON MAP
			mazeCurrent->setTileAt(target, getCurrentTile());


		*/
			mazeCurrent->getTileMap()[target.getY()][target.getX()].visited(true);		/// COMENT THIS WHEN TESTING WITH ROBOT
			start = target;
		}
		candidates = getCandidates();
		char c;
		cin >> c;
	}
	Coord returnTo = *startTile;
	Path returnPath = AStar::getPath(target, returnTo, mazeCurrent->getTileMap());
	cout << "REGRESANDO" << endl;
	returnPath.print();
}

void Cerebrum::followPath(Path& path){
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

	if(driveTrain.getDistanceLeft() < wallThreshold){
		Serial.print("L");
		result = result | wallLeftMask;
	}

	if(driveTrain.getDistanceRight() < wallThreshold){
		Serial.print("R");
		result = result | wallRightMask;
	}

	driveTrain.turnToAngle(90);

	if(driveTrain.getDistanceRight() < wallThreshold){
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

	if(driveTrain.getDistanceLeft() < wallThreshold){
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

	if(driveTrain.getDistanceRight() < wallThreshold){
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
	}else if(driveTrain.getPitch() > 5 && driveTrain.getPitch() < 20){
	//	result2 = result2 | hasBumpMask; // 
	}
	result2 = result2 | maskVisited;
	return Tile(result, result2);
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
#include "Cerebrum.h"

Cerebrum::Cerebrum(DriveTrain& driveTrain) : driveTrain(driveTrain), lcd(LCD::getInstance()){
	angles[0] = 0;
	angles[1] = 90;
	angles[2] = 180;
	angles[3] = -90;

	button1 = new Button(4);
	button2 = new Button(3);


}

void Cerebrum::start(){



  //cout <<  (int) tileMap[2][0].getCost() << " " <<  (int) tileMap[4][0].getCost() << " " <<  (int) tileMap[2][1].getCost() << endl;
  	//Map tMap(tileMap);
	//resetGyro(Back);
	Tile tile = getInitialTile();
	Map* map1 = new Map(tile, 0);

	maze.push_back(map1);
	while(!button1->getState())
		updateTelemetry();
	driveTrain.resetAll();
	delay(200);
}


void Cerebrum::run(){
	Tile* startTile = &maze[mazeFloor]->getTileMap()[0][0];
	NavigationResult navigationResult;
	Coord currentCoord  = maze[mazeFloor]->getRobotCoord();;

	while(!roundCompleted(maze)){
		navigationResult = navigateLevel(maze[mazeFloor], currentCoord);
		
		updateTelemetry();

	}
	
	Coord returnTo = maze[mazeFloor]->getBlackTileCoord();
	Path returnPath = AStar::getPath(maze[mazeFloor]->getRobotCoord(), returnTo, maze[mazeFloor]->getTileMap());
	followPath(returnPath);

	Tile blackEndTile = maze[mazeFloor]->getTileAt(currentRobotCoord);
	int angleUsed = 0;
	if(blackEndTile.wallExists(Up)){
		angleUsed = 180;
		driveTrain.turnToAngle(angleUsed);
	}else if(blackEndTile.wallExists(Right)){
		angleUsed = -90;
		driveTrain.turnToAngle(angleUsed);

	}else if(blackEndTile.wallExists(Down)){
		driveTrain.turnToAngle(angleUsed);
	}else if(blackEndTile.wallExists(Left)){
		angleUsed = 90;
		driveTrain.turnToAngle(angleUsed);
	}
	/// WAIT UNTIL SIGNAL IS RECEIVED

	driveTrain.driveDisplacement(distanceMoveTiles / 2, angleUsed, -.4, true);
	driveTrain.driveDisplacement(distanceMoveTiles / 2 , angleUsed, .4, true);
	delay(1000);
	driveTrain.driveDisplacement(distanceMoveTiles *  2, angleUsed, -.4, true);
	
	bool wallFront, silverTileReachedPassed, silverReached;

	while(!wallFront && !silverTileReachedPassed){
		driveTrain.driveStraight(angleUsed, 0.5);
		Color tileColor = driveTrain.getTileColor();
		if(tileColor == Silver){
			silverReached = true;
		}else if(tileColor == White){
			if(silverReached){
				silverTileReachedPassed = true;
			}
		}
		int distanceFront = driveTrain.getDistanceFront();
		wallFront = distanceFront < driveTrain.getDesiredWallDistance();
	}
	driveTrain.turn(0);

	if(silverTileReachedPassed){
		while(driveTrain.getTileColor() != Silver){
			driveTrain.driveStraight(angleUsed, -0.5);
		}
		delay(500);
		driveTrain.turn(0);
	}

}


Path Cerebrum::getPathLowerCost(Coord start, Absis<Coord> targets, Map* map){
		uint8_t minorCost = 254;
		Path bestPath;
		for(int i = 0; i < targets.size(); ++i){
			Coord end = targets[i];
			Path candidatePath = AStar::getPath(start, end, map->getTileMap());
			uint8_t cost = candidatePath.getCost();
			cout << "CANDIDATE COST= " << (int) cost <<"\tx: " <<(int)  end.getX() << "\ty: " <<(int)  end.getY() << endl;
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
	Absis<Coord> candidates = mapCurrent->getCandidates();
	Coord start = mapCurrent->getTileAt(startCoord);
	Coord target;
	while(candidates.size() > 0){

		Path bestPath = getPathLowerCost(start, candidates, mapCurrent);

		if(bestPath.getValid()){

			target = bestPath.getCoordAt(bestPath.getLength()-1);
			//TODO= SCAN TILE AND UPDATE MAP.TEST ONCE WE HAVE ROBOT
			followPath(bestPath);
			mapCurrent->setRobotCoord(target);

			Tile tile = getCurrentTile();
			mapCurrent->setTileAt(target,tile);		

		}

		currentRobotCoord = mapCurrent->getRobotCoord();
		start = currentRobotCoord;
		candidates = mapCurrent->getCandidates();
		updateTelemetry();
	}
	data.endCoord = target;

	if(mapCurrent->wasCompleted()){
		data.endReason = Completed;
	}
	return data;
	
}

void Cerebrum::followPath(Path& path){

	Tile currentTile = maze[mazeFloor]->getTileAt(maze[mazeFloor]->getRobotCoord());

	for(int i = 0; i < path.getLength(); ++i){
		Tile& tile = maze[mazeFloor]->getTileAt(currentRobotCoord);
		driveTrain.setShouldDispense(!tile.getLeftKit());

		Coord coord = path.getCoordAt(i);
		int deltaX = coord.getX() - maze[mazeFloor]->getRobotCoord().getX();
		int deltaY = coord.getY() - maze[mazeFloor]->getRobotCoord().getY();

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
				turnRobot(Left);
			}else if(currentRobotDirection == Down){
				turnRobot(Right);
				turnRobot(Right);
			}else if(currentRobotDirection == Left){
				turnRobot(Right);
			}
			driveForward();
		}else if(deltaY < 0){
			if(currentRobotDirection == Right){
				turnRobot(Right);
			}else if(currentRobotDirection == Left){
				turnRobot(Left);
			}else if(currentRobotDirection == Up){
				turnRobot(Left);
				turnRobot(Left);
			}
			driveForward();
		}
		driveTrain.moveDesiredDistanceToWall(preciseMovementSpeed);
	}
	
	driveTrain.turnToAngle(angles[0]);
}

void Cerebrum::driveForward(){
	currentRobotCoord = maze[mazeFloor]->getRobotCoord();
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
	driveTrain.moveDesiredDistanceToWall(preciseMovementSpeed);
	bool canCorrectBack = ((driveTrain.getDistanceBack() < 15 && driveTrain.getDistanceBack() != 0)  && currentRobotDirection == Up );
	bool canCorrectFront = ((driveTrain.getDistanceFront() < 15 && driveTrain.getDistanceFront() != 0)  && currentRobotDirection == Up );

	if( (canCorrectBack || canCorrectFront )&&  turnCounter > limitTurnCounter){
		if(maze[mazeFloor]->getTileAt(maze[mazeFloor]->getRobotCoord()).getCost() < 2){
			turnCounter = 0;
			if(canCorrectBack)
				resetGyro(Back);

		}
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
			if(currentRobotDirection == Up){
				currentRobotDirection = Right;
			}else if(currentRobotDirection == Right){
				currentRobotDirection = Down;
			}else if(currentRobotDirection == Left){
				currentRobotDirection = Up;
			}else if(currentRobotDirection == Down){
				currentRobotDirection = Left;
			}
		break;

		case Left:
			angles[0] = fourth;
			angles[1] = first;
			angles[2] = second;
			angles[3] = third;
			if(currentRobotDirection == Up){
				currentRobotDirection = Left;
			}else if(currentRobotDirection == Right){
				currentRobotDirection = Up;
			}else if(currentRobotDirection == Left){
				currentRobotDirection = Down;
			}else if(currentRobotDirection == Down){
				currentRobotDirection = Right;
			}
		break;
	};	
}

Tile Cerebrum::getInitialTile(){

	Tile tile;

	if(driveTrain.getDistanceFront() < wallThreshold){
		Serial.print("F");
		tile.setWall(Up, true);
	}

	if(driveTrain.getDistanceLeftFront() < wallThreshold){
		Serial.print("L");
		tile.setWall(Left, true);
	}

	if(driveTrain.getDistanceRightFront() < wallThreshold){
		Serial.print("R");
		tile.setWall(Right, true);
	}


	if(driveTrain.getDistanceBack() < wallThreshold){
		Serial.print("B");

		tile.setWall(Down, true);
	}
	Serial.println();


	Color sensedColor = driveTrain.getTileColor();
	switch (sensedColor) {
		case White:
			tile.setColor(White);
		break;

		case Black:
			tile.setColor(Black);
		break;

		case Silver:
			tile.setColor(Silver);
		break;

		default:
		break;
	}
	tile.visited(true);
	return tile;
}

Tile Cerebrum::getCurrentTile(){
	Tile tile;
	if(driveTrain.getDistanceFront() < wallThreshold){
		switch(currentRobotDirection){
			case Up:
				tile.setWall(Up, true);
			break;

			case Down:
				tile.setWall(Down, true);
			break;

			case Right:
				tile.setWall(Right, true);

			break;

			case Left:
				tile.setWall(Left, true);
			break;

			default:
			break;
		}
	}

	if(driveTrain.getDistanceLeftFront() < wallThreshold){
		switch(currentRobotDirection){
			case Up:
				tile.setWall(Left, true);
			break;

			case Down:
				tile.setWall(Right, true);
			break;

			case Right:
				tile.setWall(Up, true);
			break;

			case Left:
				tile.setWall(Down, true);
			break;

			default:
			break;
		}
	}

	if(driveTrain.getDistanceRightFront() < wallThreshold){
		switch(currentRobotDirection){
			case Up:
				tile.setWall(Right, true);
			break;

			case Down:
				tile.setWall(Left, true);
			break;

			case Right:
				tile.setWall(Down, true);

			break;

			case Left:
				tile.setWall(Up, true);
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
			tile.setColor(Black);
		break;

		case Silver:
			tile.setColor(White);
		break;

		default:
		break;
	}

	if(driveTrain.getPitch() >= rampAngle)
		tile.setRamp(true);
	
	tile.visited(true);

	int angleDiff = driveTrain.getAngleDiffPitchHistory();

	tile.setBumpLevel(Flat);
	if(angleDiff >= lowBumpAngleDiff && angleDiff < mediumAngleDiff){
		tile.setBumpLevel(Small);

	}else if(angleDiff >=  mediumAngleDiff && angleDiff < maxAngleDiff){
		tile.setBumpLevel(Medium);

	}else if(angleDiff > maxAngleDiff){
		tile.setBumpLevel(Max);

	}
	
	if(driveTrain.leftKitLastMovement()){
		tile.setLeftKit(true);
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

void Cerebrum::resetGyro(RobotFace dir){
	const double distanceMoveStart = driveTrain.getDesiredWallDistance();
	driveTrain.turnToAngle(0);
	driveTrain.alignWithWall(dir);
	driveTrain.resetAll();
	if(dir == Back)
		driveTrain.driveDisplacement(distanceMoveStart,angles[0], preciseMovementSpeed);
	else if(dir == Front)
		driveTrain.driveDisplacement(distanceMoveStart,angles[0], -preciseMovementSpeed);

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

void Cerebrum::updateTelemetry(){
	String str;
	
	Tile currentTile = maze[mazeFloor]->getTileAt(maze[mazeFloor]->getRobotCoord());
	str.concat("X:");
	str.concat(maze[mazeFloor]->getRobotCoord().getX());
	str.concat("Y:");
	str.concat(maze[mazeFloor]->getRobotCoord().getY());
	str.concat(" ");
	str.concat(currentTile.wallExists(Up));
	str.concat(currentTile.wallExists(Right));
	str.concat(currentTile.wallExists(Down));
	str.concat(currentTile.wallExists(Left));
	str.concat("W:");
	str.concat(maze[mazeFloor]->getWidth());
	str.concat("H:");
	str.concat(maze[mazeFloor]->getHeight());
	str.concat("NV:");
	str.concat(maze[mazeFloor]->getNonVisitedTiles());
	switch(currentRobotDirection){
		case Up:{
			str.concat("U");
		}
		break;
		
		case Right:{
			str.concat("R");
		}
		break;		

		case Down:{
			str.concat("D");
		}
		break;		
		
		case Left:{
			str.concat("L");
		}
		break;
	}
	str.concat("F:");
	str.concat(mazeFloor);
	str.concat("C:");
	str.concat(driveTrain.getTileColor());
	lcd.display(str);
	//lcd.display(freeMemory());
}
void Cerebrum::showVisitedTilesLCD(){
	Absis<Absis<Tile>> tileMap = maze[mazeFloor]->getTileMap();
	for(int y = 0; y < tileMap.size(); ++y){
		for(int x = 0; x < tileMap[0].size(); ++x){
			if(tileMap[y][x].wasVisited()){
				Tile tile = tileMap[y][x];
				String str;
				str.concat("VIS: ");
				str.concat(tile.wallExists(Up));
				str.concat(tile.wallExists(Right));
				str.concat(tile.wallExists(Down));
				str.concat(tile.wallExists(Left));
				str.concat(" ");
				str.concat(x);
				str.concat(" ");
				str.concat(y);
				str.concat(" ");
				str.concat(tile.getX());
				str.concat(" ");
				str.concat(tile.getY());
				lcd.display(str);
				while(!button1->getState()){
					delay(100);
				}
				delay(100);
			}

		}
	} 
}

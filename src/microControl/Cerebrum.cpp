#include "Cerebrum.h"

Cerebrum::Cerebrum(DriveTrain& driveTrain) : driveTrain(driveTrain), lcd(LCD::getInstance()){
	angles[0] = 0;
	angles[1] = 90;
	angles[2] = 180;
	angles[3] = -90;
	

}

void Cerebrum::start(){
	resetGyro();
	maze[mazeFloor] = (new Map(getInitialTile()));
}

void Cerebrum::prepareForMove(){
///	Pre-movement checks and setup
	currentRobotCoord = maze[mazeFloor]->getRobotCoord();
	maze[mazeFloor]->expandMap();

	if(hasStarted){
		tile = getCurrentTile();
	}else{
		hasStarted = true;
		tile = maze[mazeFloor]->getTileAt(currentRobotCoord);
	}

	updateRobotOrientations();

	if(tile.isRamp()){
		while(driveTrain.getPitch() > 10){
			driveTrain.driveStraight(angles[0], rampMovementSpeed);
		}
		driveTrain.driveDisplacement(8, angles[0], movementSpeed);
		if(mazeFloor < 3)
			mazeFloor++;
		maze[mazeFloor] = (new Map(getCurrentTile()));
		maze[mazeFloor]->expandMap();
		currentRobotCoord = maze[mazeFloor]->getRobotCoord();
	}

	if(turnCounter >= 5 && tile.wallExists(Down)){
		double lastAngle = angles[0];
		resetGyro();
		driveTrain.turnToAngle(lastAngle);
		turnCounter = 0;
	}

	if(tile.wallExists(robotUp)){
		while(driveTrain.getDistanceFront() != wallTolerance){
			if(driveTrain.getDistanceFront() > wallTolerance)
				driveTrain.driveStraight(angles[0], preciseMovementSpeed);
			else if(driveTrain.getDistanceFront() < wallTolerance)
				driveTrain.driveStraight(angles[0], preciseMovementSpeed * -1);
		}
		driveTrain.driveVelocity(0);
	}
}

void Cerebrum::move(){
///	Decide where to move
	if(!tile.wallExists(robotUp) && getRobotTile(Up).getColor() != Black){
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
		while(driveTrain.getDistanceFront() < wallThreshold){
			turnRobot(Right);
		}
		driveForward();
	}

}

void Cerebrum::updateAfterMove(){
///	After-movement checks and setup
	if(driveTrain.leftKitLastMovement()){
		Tile oldTile = maze[mazeFloor]->getTileAt(currentRobotCoord);
		oldTile.setLeftKit(true);
		maze[mazeFloor]->setTileAt(currentRobotCoord, oldTile);
	}
	driveTrain.setLeftKit(false);	

}

vector<Tile*> Cerebrum::getCandidates(){
	vector <Tile*> vec;
	Map* maze = this->maze[mazeFloor];
	for (int y = 0; y < maze->size(); ++y) {
		for (int x = 0; x < maze[0]->size(); ++x) {
			if (maze[y][x]->getWasVisited()) {

				//cout << "LA TILE VISITADA ES (" << x << "," << y << ")" << endl;
				vector <Node*> neighbors = maze[y][x]->getNeighbors();

				//cout << "SUS VECINOS NO VISITADOS SON : ";

				for (int i = neighbors.size() - 1; i >= 0; --i) {
					if (!neighbors[i]->getWasVisited()) {
						Node* elem = neighbors[i];
						if (!isInVector(vec, elem)) {
							vec.emplace_back(neighbors[i]);
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

void Cerebrum::update(){
	vector<Tile*> candidates = getCandidates();
	Tile* start = &maze[0][0];

	Tile* returnTo = start;
	while(candidates.size() > 0){
		Tile* target = getClosestFrom(candidates, start);
		cout << "StartX: " << start->getX() << " StartY: " << start->getY() << endl;
		cout << "EndX: " << target->getX() << " EndY: " << target->getY() << endl;

		Path pathToFollow = AStar::getPath(maze, *start, *end);
		//TODO FOLLOW PATH
		
		target->setWasVisited(true);
		start = target;
		candidates = getCandidates(maze);

	}

	// prepareForMove();
	// move();
	// updateAfterMove();
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
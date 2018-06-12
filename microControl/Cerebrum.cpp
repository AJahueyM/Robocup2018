#include "Cerebrum.h"

Cerebrum::Cerebrum(DriveTrain& driveTrain) : driveTrain(driveTrain), lcd(LCD::getInstance()){
	angles[0] = 0;
	angles[1] = 90;
	angles[2] = 180;
	angles[3] = -90;

	button1 = new Button(4);
	button2 = new Button(3);


}

Path Cerebrum::getPathLowerCost(Coord start, vector<Coord> targets, Map* map){
		uint8_t minorCost = 254;
		Path bestPath;
		for(Coord end  : targets){
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
	vector<Coord> candidates = mapCurrent->getCandidates();
	Coord start = mapCurrent->getTileAt(startCoord);
	Coord target;

	
	while(candidates.size() > 0){

		vector<Coord> targets;
		

		targets = getClosestFrom(candidates, start);

		Path bestPath = getPathLowerCost(start, targets, mapCurrent);


		if(bestPath.getValid()){

			target = bestPath.getCoordAt(bestPath.getLength()-1);
			//TODO= SCAN TILE AND UPDATE MAP.TEST ONCE WE HAVE ROBOT


			followPath(bestPath);
			mapCurrent->setRobotCoord(target);

			Tile tile =  getCurrentTile();

			mapCurrent->setTileAt(target,tile);
			currentRobotCoord = mapCurrent->getRobotCoord();
			start = currentRobotCoord;

			if(mapCurrent->getTileAt(target).isRamp()){

				data.endCoord = target;
				data.endReason = RampReached;
				return data;
			}
		}
		//mapCurrent->getTileAt(target).visited(true);		/// COMENT THIS WHEN TESTING WITH ROBOT

		candidates = mapCurrent->getCandidates();
		//updateTelemetry();

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

		
		followPath(pathToClosestRamp);
		
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



  //cout <<  (int) tileMap[2][0].getCost() << " " <<  (int) tileMap[4][0].getCost() << " " <<  (int) tileMap[2][1].getCost() << endl;
  	//Map tMap(tileMap);
	resetGyro();
	Tile tile = getInitialTile();
	Map* map1 = new Map(tile, 0);

	maze.push_back(map1);
	while(!button1->getState())
		updateTelemetry();
	delay(200);
}


void Cerebrum::run(){

	Tile* startTile = &maze[mazeFloor]->getTileMap()[0][0];
	NavigationResult navigationResult;
	Coord currentCoord  = maze[mazeFloor]->getRobotCoord();;

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
		//RUN PATH
		followPath(pathToRamp);
		
		mazeFloor = bestRamp.getTargetFloor();
		currentCoord = bestRamp.getEnd();
		currentRobotCoord = currentCoord;
		/*UPDATE ROBOT COORD
		
		*/
	}
	
	Coord returnTo = maze[mazeFloor]->getOriginCoord();
	Path returnPath = AStar::getPath(maze[mazeFloor]->getRobotCoord(), returnTo, maze[mazeFloor]->getTileMap());
	followPath(returnPath);


}

void Cerebrum::followPath(Path& path){

	Tile currentTile = maze[mazeFloor]->getTileAt(maze[mazeFloor]->getRobotCoord());
	driveTrain.setShouldDispense(!currentTile.getLeftKit());

	for(int i = 0; i < path.getLength(); ++i){
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
	updateTelemetry();
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

	if(((driveTrain.getDistanceBack() < 15 && driveTrain.getDistanceBack() != 0)  && currentRobotDirection == Up )&&  turnCounter > limitTurnCounter){
		if(maze[mazeFloor]->getTileAt(maze[mazeFloor]->getRobotCoord()).getCost() < 2){
			turnCounter = 0;
			resetGyro();
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
	Tile tile;
	if(driveTrain.getDistanceFront() < wallThreshold){
		//Serial.print("F");
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
		//Serial.print("L");

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
		//Serial.print("R");

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
	// Serial.println();

	if(driveTrain.getPitch() > 0 ){
		if(driveTrain.getPitch() > 20)
			tile.setRamp(true);
	}
	cout << "IN GET CURRENTTILE: " << tile.wasVisited() << endl;
	tile.visited(true);
	cout << "IN GET CURRENTTILE AFTER: " << tile.wasVisited() << endl;

	Absis<int> pitchHistory = driveTrain.getPitchHistory();
	int lowestAngle = 300, highestAngle = -300;

	for(int i = 0; i < pitchHistory.size(); ++i){
		int angle = pitchHistory[i];
		if(angle < lowestAngle){
			lowestAngle = angle;
		}
		if(angle > highestAngle){
			highestAngle = angle;
		}
	}
	int angleDiff = abs(highestAngle - lowestAngle);

	tile.setBumpLevel(Flat);
	cout << "INTER GETCURRENTTILE= " << tile.wasVisited() << endl;
	if(angleDiff > lowBumpAngleDiff){
		tile.setBumpLevel(Small);
	cout << "INTER GETCURRENTTILE= " << tile.wasVisited() << endl;

	}
	if(angleDiff > mediumAngleDiff){
		tile.setBumpLevel(Medium);
		cout << "INTER GETCURRENTTILE= " << tile.wasVisited() << endl;

	}
	if(angleDiff > maxAngleDiff){
		tile.setBumpLevel(Max);
		cout << "INTER GETCURRENTTILE= " << tile.wasVisited() << endl;

	}
	
	if(driveTrain.leftKitLastMovement()){
		tile.setLeftKit(true);
	}else{
		driveTrain.setLeftKit(false);
	}
	cout << "FINAL GETCURRENTTILE= " << tile.wasVisited() << endl;
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
	const double distanceMoveStart = driveTrain.getDesiredWallDistance();
	driveTrain.turnToAngle(0);
	driveTrain.alignWithWall(Back);
	driveTrain.resetAll();
	driveTrain.driveDisplacement(distanceMoveStart,angles[0], preciseMovementSpeed);
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
	str.concat(" Y:");
	str.concat(maze[mazeFloor]->getRobotCoord().getY());
	str.concat(" ");
	str.concat(currentTile.wallExists(Up));
	str.concat(currentTile.wallExists(Right));
	str.concat(currentTile.wallExists(Down));
	str.concat(currentTile.wallExists(Left));
	str.concat(" W:");
	str.concat(maze[mazeFloor]->getWidth());
	str.concat(" H:");
	str.concat(maze[mazeFloor]->getHeight());
	str.concat(" ");
	str.concat(maze[mazeFloor]->getNonVisitedTiles());
	str.concat(" ");
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
	str.concat(mazeFloor);
	lcd.display(str);
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

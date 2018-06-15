#ifndef __CEREBRUM_H_
#define __CEREBRUM_H_
#include "Map.h"
#include "LCD.h"
#include "Utils.h"
#include "ArduinoSTL.h"
#include "AStar.h"
#include "TileMasks.h"
#include "DriveTrain.h"
#include "Button.h"
#include <MemoryFree.h>

enum StopReason {
	Completed,
	NoCandidates,
	RampReached
};

struct NavigationResult {
	Coord endCoord;
	StopReason endReason;
};

class Cerebrum{
public:
	static Cerebrum& getInstance(DriveTrain& driveTrain){
		static Cerebrum singletonInstance(driveTrain);
		return singletonInstance;
	}
	void start();
	void run();
private:
	Cerebrum(DriveTrain& driveTrain);
    Cerebrum &operator=(const Cerebrum &);
	Path getPathLowerCost(Coord start, Absis<Coord> targets, Map* map);
	bool roundCompleted(Absis<Map*> maps);
	NavigationResult navigateLevel(Map* mapCurrent, Coord startCoord);
	void followPath(Path& path);
    void driveForward();
    void turnRobot(Direction dir);
    void shiftAngle(Direction dir);
	Tile getInitialTile();
	Tile getCurrentTile();
	Tile getRobotTile(Direction dir);
	void resetGyro(RobotFace dir);
	void updateRobotOrientations();

	void updateTelemetry();
	void showVisitedTilesLCD();
    DriveTrain& driveTrain;
    Direction currentRobotDirection = Up;
   	Direction robotRight, robotLeft, robotUp, robotDown;
    Coord currentRobotCoord;
    Absis<Map*> maze;
    LCD& lcd;
	Button *button1, *button2;
    double movementSpeed = .75, preciseMovementSpeed = .35, rampMovementSpeed = .4;
 	uint8_t distanceMoveTiles = 30, rampAngle = 20,  turnCounter = 0, mazeFloor = 0, wallTolerance = 9, wallThreshold = 15;
	short int  angles[4];
	int lowBumpAngleDiff = 2, mediumAngleDiff = 4, maxAngleDiff = 8, limitTurnCounter = 6;
 	bool hasStarted = false;
};
#endif
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
	void completeLevel();
	void followPath(Path& path);
    void driveForward();
    void turnRobot(Direction dir);
    void shiftAngle(Direction dir);
	Tile getInitialTile();
	Tile getCurrentTile();
	Tile getRobotTile(Direction dir);
	void resetGyro();
	void updateRobotOrientations();

	vector<Coord> getCandidates();
	Tile tile;
    DriveTrain& driveTrain;
    Direction currentRobotDirection = Up;
   	Direction robotRight, robotLeft, robotUp, robotDown;
    Coord currentRobotCoord;
    Map* maze[3];
    LCD& lcd;
	Button *button1, *button2;
    double movementSpeed = .75, preciseMovementSpeed = -.25, rampMovementSpeed = .5;
 	uint8_t distanceMoveTiles = 30, turnCounter = 0, mazeFloor = 0, wallTolerance = 9, wallThreshold = 15;
	short int  angles[4];
 	bool hasStarted = false;
};
#endif
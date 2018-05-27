#ifndef __CEREBRUM_HEADER_
#define __CEREBRUM_HEADER_
#include "Map.h"
#include "LCD.h"
#include "Utils.h"
#include "ArduinoSTL.h"
#include "DriveTrain.h"

class Cerebrum{
public:
	static Cerebrum& getInstance(DriveTrain& driveTrain){
		static Cerebrum singletonInstance(driveTrain);
		return singletonInstance;
	}
	void start();
	void update();
private:
	Cerebrum(DriveTrain& driveTrain);
    Cerebrum &operator=(const Cerebrum &);
    void driveForward();
    void turnRobot(Direction dir);
    void shiftAngle(Direction dir);
	Tile getInitialTile();
	Tile getCurrentTile();
	Tile getRobotTile(Direction dir);
	void resetGyro();
	void updateRobotOrientations();

	void prepareForMove();
	void move();
	void updateAfterMove();
	Tile tile;
    DriveTrain& driveTrain;
    Direction currentRobotDirection = Up;
   	Direction robotRight, robotLeft, robotUp, robotDown;
    Coord currentRobotCoord;
    vector<Map*> maze;
    LCD& lcd;

    double movementSpeed = .75, preciseMovementSpeed = -.25, rampMovementSpeed = .5;
 	int distanceMoveTiles = 30, angles[4], turnCounter = 0, mazeFloor = 0, wallTolerance = 9, wallThreshold = 15;
 	bool hasStarted = false;
};

#endif
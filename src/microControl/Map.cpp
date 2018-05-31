#include "Map.h"

Map::Map(Absis<Absis<Tile>>& tileMap){
	this->tileMap = tileMap;
	updateMap();
}

Map::Map(Tile initialTile){
	tileMap.push_back(Absis<Tile>());
	tileMap[0].push_back(initialTile);
}

Tile& Map::getTileAt(Coord coord){
	if(coord.getY()> tileMap.size() - 1 || coord.getY() < 0){
		return mockTile;
	}
	if(coord.getX()> tileMap[0].size() - 1 || coord.getX() < 0){
		return mockTile;
	}
	return tileMap[coord.getY()][coord.getX()];
}

void Map::setTileAt(Coord coord, Tile newTile){
	tileMap[coord.getY()][coord.getX()] = newTile;
	updateMap();
}

Coord Map::getRobotCoord(){
	return this->robotCoord;
}

void Map::expandMap(){
	Serial.print("X:");
	Serial.print(getRobotCoord().getX());
	Serial.print("Y:");
	Serial.println(getRobotCoord().getY());

	if(getRobotCoord().getX()  == getWidth() - 1){
		if(!getTileAt(getRobotCoord()).wallExists(Right)){
			for(int i = 0; i < tileMap.size(); ++i){
				tileMap[i].push_back(Tile());
			}
		}
	}

	if(getRobotCoord().getX() == 0){
		if(!getTileAt(getRobotCoord()).wallExists(Left)){
			for(int i = 0; i < tileMap.size(); ++i){
				tileMap[i].addStart(Tile());
			}
			Coord newRobotCoord = getRobotCoord();
			newRobotCoord.setX(1);
			setRobotCoord(newRobotCoord);
		}
	}

	if(getRobotCoord().getY() == 0){
		if(!getTileAt(getRobotCoord()).wallExists(Down)){
			Absis<Tile> newRow;
			for(int i = 0; i < tileMap[0].size(); ++i){
				newRow.push_back(Tile());
			}
     		tileMap.addStart( newRow);
			Coord newRobotCoord = getRobotCoord();
			newRobotCoord.setY(1);
			setRobotCoord(newRobotCoord);
		}
	}

	if(getRobotCoord().getY() == getHeight() - 1){
		if(!getTileAt(getRobotCoord()).wallExists(Up)){
			Absis<Tile> newRow;
			for(int i = 0; i < tileMap[0].size(); ++i){
				newRow.push_back(Tile());
			}
			tileMap.push_back(newRow);
		}
	}
	updateMap();
}

void Map::setRobotCoord(Coord coord){
	this->robotCoord = coord;
}

Path Map::getPathTo(Coord targetCoord){

}

uint8_t Map::getWidth(){
	return tileMap[0].size();
}

uint8_t Map::getHeight(){
	return tileMap.size();
}

int Map::getNonVisitedTiles(){
	int counter = 0;

	for(int y = 0; y < tileMap.size(); ++y){
		for(int x = 0; x < tileMap[0].size();++x){
			if(!tileMap[y][x].wasVisited())
				counter++;
		}
	}
	return counter;
}

void Map::updateCoords(){
	for(int y = 0; y < tileMap.size(); ++y){
		for(int x = 0; x < tileMap[0].size(); ++x){
			tileMap[y][x].setX(x);
			tileMap[y][x].setY(y);
		}
	}	
}

void Map::updateWalls(){
    for(int y = 0; y < tileMap.size(); ++y){
        for(int x = 0; x < tileMap[0].size() ; ++x){
            Tile &node = tileMap[y][x];

            if(y - 1 >= 0 && node.wallExists(Up))
                tileMap[y-1][x].setWall(Down, true);
            if(y + 1 < tileMap.size() && node.wallExists(Down))
                tileMap[y+1][x].setWall(Up, true);
            if(x + 1 < tileMap[0].size() && node.wallExists(Right))
                tileMap[y][x+1].setWall(Left, true);
            if(x - 1 >= 0 && node.wallExists(Left))
                tileMap[y][x-1].setWall(Right, true);
        }
    }
}

void Map::checkPockets(){
	for(int y = 0; y < tileMap.size() ;++y){
		for(int x = 0; x < tileMap[0].size(); ++x){
			Coord left, up, right, down;
			up.setX(x);
			up.setY(y + 1);

			down.setX(x);
			down.setY(y-1);

			right.setX(x +1);
			right.setY(y);

			left.setX(x - 1);
			left.setY(y);

			bool pocket = (getTileAt(up).wallExists(Down) && getTileAt(down).wallExists(Up)) && (getTileAt(right).wallExists(Left) && getTileAt(left).wallExists(Right));
			if(pocket){
				setTileAt(Coord(x, y), mockTile);
			}
		}
	}
}

void Map::updateNeighbors(){
	for(int y = 0; y < tileMap.size(); ++y){
		Absis<Tile>& row = tileMap[y];
		for(int x = 0; x < tileMap[0].size(); ++x){
			Tile& node = row[x];

			if(y - 1 >= 0 && !node.wallExists(Up))
				node.addNeighbor(&tileMap[y-1][x]);
			if(y + 1 < tileMap.size() && !node.wallExists(Down))
				node.addNeighbor(&tileMap[y+1][x]);
			if(x + 1 < tileMap[0].size() && !node.wallExists(Right))
				node.addNeighbor(&tileMap[y][x+1]);
			if(x - 1 >= 0 && !node.wallExists(Left))
				node.addNeighbor(&tileMap[y][x-1]);
		}
	}
}

void Map::updateMap(){
	updateCoords();
	checkPockets();
	updateWalls();
	updateNeighbors();
}
Absis<Absis<Tile>>& Map::getTileMap(){
	return this->tileMap;
}

void Map::setTileMap(Absis<Absis<Tile>> tileMap){
	this->tileMap = tileMap;
	updateMap();
}
#include "Map.h"

Map::Map(Tile initialTile){
	tileMap.push_back(vector<Tile>());
	tileMap[0].push_back(initialTile);
	mockTile = Tile(mockIdentity, mockIdentity);
}

void Map::addColumn(Direction side){

}

void Map::addRow(Direction side){

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
				tileMap[i].insert(tileMap[i].begin(),Tile());
			}
			Coord newRobotCoord = getRobotCoord();
			newRobotCoord.setX(1);
			setRobotCoord(newRobotCoord);
		}
	}

	if(getRobotCoord().getY() == 0){
		if(!getTileAt(getRobotCoord()).wallExists(Down)){
			vector<Tile> newRow;
			for(int i = 0; i < tileMap[0].size(); ++i){
				newRow.push_back(Tile());
			}
     		tileMap.insert(tileMap.begin(), newRow);
			Coord newRobotCoord = getRobotCoord();
			newRobotCoord.setY(1);
			setRobotCoord(newRobotCoord);
		}
	}

	if(getRobotCoord().getY() == getHeight() - 1){
		if(!getTileAt(getRobotCoord()).wallExists(Up)){
			vector<Tile> newRow;
			for(int i = 0; i < tileMap[0].size(); ++i){
				newRow.push_back(Tile());
			}
			tileMap.push_back(newRow);
		}
	}

	checkPockets();
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
	for(vector<Tile> row : tileMap){
		for(Tile t : row){
			if(!t.wasVisited())
				counter++;
		}
	}
	return counter;
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

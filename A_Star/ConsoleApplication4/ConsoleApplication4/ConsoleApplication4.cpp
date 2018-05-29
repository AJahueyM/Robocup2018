// ConsoleApplication4.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <vector>
#include <iostream>
#include "windows.h"

using namespace std;

#include "Node.h"

template<class T>
void removeFromVector(vector <T> &vec, T elem) {

	for (int i = vec.size() - 1; i >= 0; --i) {
		if (vec[i] == elem) {
			vec.erase(vec.begin() + i);
			break;
		}
	}
}

void addNeighbors(Node *node, vector<vector<Node>> &maze ) {
	int x = node->getX();
	int y = node->getY();
	int cols = maze[0].size();
	int rows = maze.size();

	if (x < cols - 1) {

		if (!maze[y][x].getHasWallRight()) {
			node->addNeighbor(&maze[y][x + 1]);
		}
	}

	if (x > 0) {

		if (!maze[y][x].getHasWallLeft()) {
			node->addNeighbor(&maze[y][x - 1]);
		}
	}
	if (y < rows - 1) {

		if (!maze[y][x].getHasWallDown()) {
			node->addNeighbor(&maze[y + 1][x]);
		}
	}
	if (y > 0) {

		if (!maze[y][x].getHasWallUp()) {
			node->addNeighbor(&maze[y - 1][x]);
		}
	}
}

template <class T>
bool isInVector(vector<T> &vec, T &elem) {
	for (T e : vec) {
		if (e == elem) {
			return true;
		}
	}

	return false;
}

int calculateDistance(Node *a, Node *b) {
	int dx = abs(a->getX() - b->getX());
	int dy = abs(a->getY() - b->getY()); 
	return (dx + dy); 
}

void updateWalls(vector <vector<Node>> &maze) {
	int cols = maze[0].size();
	int rows = maze.size();
	
	for (int y = 0; y < maze.size(); ++y) {
		for (int x = 0; x < maze[0].size(); ++x) {	
			if (x - 1 < cols - 1) {

				if (maze[y][x].getHasWallRight()) {
					maze[y][x + 1].setHasWallLeft(true);
				}
			}

			if (x + 1 > 0) {

				if (maze[y][x ].getHasWallLeft()) {
					maze[y][x - 1].setHasWallRight(true);
				}
	
			}
			if (y -  1 < rows - 1) {
				
				if (maze[y][x].getHasWallDown()) {
					maze[y + 1][x].setHasWallUp(true);
				}
			}
			if (y + 1 > 0) {

				if (maze[y][x].getHasWallUp()) {
					maze[y - 1][x].setHasWallDown(true);
				}

			}
		}
	}
}

void printMaze(vector <vector <Node>> maze) {
	cout << "PRINTING MAZE...";
	for (int y = 0; y < maze.size(); ++y) {
		cout << endl;
		for (int x = 0; x < maze[0].size(); ++x) {
			if (maze[y][x].getHasWallUp()) {
				cout << " UP ";
			}
			else {
				cout << "|--|";
			}

			if (maze[y][x].getHasWallDown()) {
				cout << " DN ";
			}
			else {
				cout << "|--|";
			}
			if (maze[y][x].getHasWallLeft()) {
				cout << " LF ";
			}
			else {
				cout << "|--|";
			}
			if (maze[y][x].getHasWallRight()) {
				cout << " RT ";
			}
			else {
				cout << "|--|";
			}
			cout << "/";

		}
	}
}

int main(){
	
	vector <vector<Node>> maze;
	int cols = 5, rows = 5;

	for (int y = 0; y < rows; ++y) {
		maze.emplace_back();
		for (int x = 0; x < cols; ++x) {
			maze[y].emplace_back(Node(x,y));
		}
	}

	maze[0][0].setHasWallRight(true);
	maze[1][0].setHasWallRight(true);
	maze[2][0].setHasWallDown(true);
	maze[4][0].setHasWallRight(true);
	maze[1][1].setHasWallRight(true);
	maze[2][1].setHasWallRight(true);
	maze[2][1].setHasWallDown(true);
	maze[3][1].setHasWallRight(true);
	maze[2][2].setHasWallDown(true);
	maze[4][2].setHasWallRight(true);
	maze[2][3].setHasWallDown(true);
	maze[2][3].setHasWallRight(true);
	maze[3][3].setHasWallRight(true);
	maze[3][1].setHasWallLeft(true);

	updateWalls(maze);

	cout << endl <<"DESPUES DE EDICION " << endl;

	vector <Node*> openSet;
	vector <Node*> closedSet;

	Node* start = &maze[4][0];
	Node* end =  &maze[0][0];
	
	openSet.emplace_back(start);
	
	while (openSet.size() > 0) {
		
		int winnerIndex = 0;
		for (int i = 0; i < openSet.size(); ++i) {
			if (openSet[i]->getF() < openSet[winnerIndex]->getF()) { // checar si agregar verificacion de G closests si ==f
				winnerIndex = i;
			}
		}

		Node* current = openSet[winnerIndex];

		if (current == end) {
			cout << "DONE!" << endl;
			
			vector <Node*> path;
			Node* temp = current;
			path.emplace_back(temp);

			while (temp->getParent() != nullptr) {
				path.emplace_back(temp->getParent());
				temp = temp->getParent();
			}

			cout << "X|Y" << endl;
			for (int i = path.size() - 1; i >= 0; --i) {
				cout << path[i]->getX() << " " << path[i]->getY() << endl;
			}

			Sleep(20000);
			int i;
			cin >> i;
			return 0;
		}

		removeFromVector(openSet, current);
		closedSet.emplace_back(current);

		addNeighbors(current, maze);

		for (Node* neighbor : current->getNeighbors()) {

			if (!isInVector(closedSet, neighbor)) {
				int tempG = current->getG() + 1;

				bool newPath = false;
				if (isInVector(openSet, neighbor)) {
					if (tempG < neighbor->getG()) {
						neighbor->setG(tempG);
						newPath = true;
					}
				}
				else {
					neighbor->setG(tempG);
					openSet.emplace_back(neighbor);
					newPath = true;
				}
				if (newPath) {
					neighbor->setH(calculateDistance(neighbor, end));
					neighbor->setF(neighbor->getG() + neighbor->getH());
					neighbor->setParent(current);
				}
				
			}

		}


	}

	if (openSet.size() == 0) {
		cout << "NO SOLUTION" << endl;
	}
	Sleep(20000);
	return 0;
}


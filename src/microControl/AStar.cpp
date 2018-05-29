#include "AStar.h"

int AStar::heuristic(Node* start, Node* end){
    return abs(start->getX() - start->getX()) + abs(end->getY() - end->getY());
}

Path AStar::getPath(Coord startCoord, Coord endCoord, vector<vector<Tile>>& maze){

    int sizeX = maze[0].size();
    int sizeY = maze.size();
    cout << "SizeX= " << sizeX << " SizeY= " << sizeY << endl;
    if(startCoord.getX() < 0 || startCoord.getX() >= sizeX){
        return Path();
    }

    if(startCoord.getY() < 0 || startCoord.getY() >= sizeY){
        return Path();
    }

    if(endCoord.getX() < 0 || endCoord.getX() >= sizeX){
        return Path();
    }

    if(endCoord.getY() < 0 || endCoord.getY() >= sizeY){
        return Path();
    }

    if(startCoord == endCoord){
        return Path();
    }

    vector<Node*> openSet, closedSet;

    cout << maze[0][0].getX() << endl;
    Node *start = &maze[startCoord.getY()][startCoord.getX()], *end = &maze[endCoord.getY()][endCoord.getX()];

    openSet.push_back(start);

    cout << "Before" << endl;
    cout << start->getX() << " " << start->getY() << endl;
    cout << "TEST= " << test << endl;
    cout << "After" << endl;

    while(openSet.size() > 0){
        cout << "Free= " <<freeMemory() << endl;

        cout << "HI " << openSet.size() << endl;
        int lowestIndex = 0;
        for(int i = 0; i < openSet.size(); ++i){
            if(openSet[i]->getF() < openSet[lowestIndex]->getF()){
                lowestIndex = i;
            }
        }
        cout << "HI2" << endl;

        Node *current = openSet[lowestIndex];
        if(current == end){
            cout << "DONE" << endl;

            vector<Coord> temp;
            while(current->getPrevious() != nullptr){
                temp.push_back(Coord(current->getX(), current->getY()));
                current = current->getPrevious();
            }
            return Path(temp);
        }
        cout << "HI3" << endl;

        //cout << "Before= " << openSet.size() << endl;
        closedSet.push_back(current);
        cout << "HI4" << endl;

        //cout << countsOnVector(closedSet, current) <<  endl;
        removeFromVector(openSet, current);
        //cout << "After= " << openSet.size() << endl;
        cout << "HI5" << endl;

        vector<Node*> neighbors;
        int amountNeighbors = current->getCurrentNeighbors();
         for(int i = 0; i < amountNeighbors;++i){
            neighbors.push_back(current->getNeighbors(i));
        }

        cout << "Neighbors of " << current->getX() << " " << current->getY() << " " << neighbors.size() << endl;
        cout << "HI6" << endl;
 
        cout << "HI7" << endl;
        for(Node* neighbor : neighbors){
            cout << "HI7.1" << endl;
            if(countsOnVector(closedSet, neighbor) == 0){
                int tempG = current->getG() + 1;

                if(countsOnVector(openSet, neighbor)>= 1){
                    if(tempG < neighbor->getG()){
                        neighbor->setG(tempG);
                    }
                }else{
                    neighbor->setG(tempG);
                    openSet.push_back(neighbor);
                }
                neighbor->setH(heuristic(neighbor, end));
                neighbor->setF( neighbor->getG() + neighbor->getH());
                neighbor->setPrevious(current);
            }
        }
        cout << "HI8" << endl;
        cout << openSet.size() << endl;
    }
    cout << "No solution" << endl;
    return Path();
}
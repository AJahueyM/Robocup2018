#include "AStar.h"

int AStar::heuristic(Node* start, Node* end){
    return abs(start->getX() - start->getX()) + abs(end->getY() - end->getY());
}

Path AStar::getPath(Coord startCoord, Coord endCoord, Absis<Absis<Tile>>& maze){

    int sizeX = maze[0].size();
    int sizeY = maze.size();
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

    Node *start = &maze[startCoord.getY()][startCoord.getX()], *end = &maze[endCoord.getY()][endCoord.getX()];

    openSet.push_back(start);

    while(openSet.size() > 0){
        int lowestIndex = 0;
        for(int i = 0; i < openSet.size(); ++i){
            if(openSet[i]->getF() < openSet[lowestIndex]->getF()){
                lowestIndex = i;
            }
        }

        Node *current = openSet[lowestIndex];
        if(current == end){
          
            vector<Coord> temp, path;

            while(current->getPrevious() != nullptr){
                temp.push_back(Coord(current->getX(), current->getY()));
                current = current->getPrevious();
            }

            for(int i = temp.size()-1; i >= 0; --i){
                path.push_back(temp[i]);
            }
            return Path(path);
        }

        //cout << "Before= " << openSet.size() << endl;
        closedSet.push_back(current);

        //cout << countsOnVector(closedSet, current) <<  endl;
        removeFromVector(openSet, current);
        //cout << "After= " << openSet.size() << endl;

       Node* neighbors[4];
        int amountNeighbors = current->getCurrentNeighbors();
         for(int i = 0; i < amountNeighbors;++i){
            neighbors[i] = (current->getNeighbors(i));
        }

        for(int i = 0; i < amountNeighbors; ++i){
            Node* neighbor = neighbors[i];
            if(countsOnVector(closedSet, neighbor) == 0){
                
                int tempG = current->getG() + current->getCost();

                bool update = false;

                if(countsOnVector(openSet, neighbor)>= 1){
                    if(tempG < neighbor->getG()){
                        neighbor->setG(tempG);
                        update = true;

                    }
                }else{
                    neighbor->setG(tempG);
                    openSet.push_back(neighbor);
                    update = true;
                }
                if(update){
                    neighbor->setH(heuristic(neighbor, end));
                    neighbor->setF( neighbor->getG() + neighbor->getH());
                    neighbor->setPrevious(current);
                }
            }
        }
    }
    cout << "No solution" << endl;
    return Path();
}
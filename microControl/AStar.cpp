#include "AStar.h"

int AStar::heuristic(Coord start, Coord end){
    return abs(start.getX() - start.getX()) + abs(end.getY() - end.getY());
}

Path AStar::getPath(Coord startCoord, Coord endCoord, Absis<Absis<Tile>>& maze){

    int sizeX = maze[0].size();
    int sizeY = maze.size();
    Path pathFinal;
    pathFinal.setCost(254);
    pathFinal.setValid(false);

    if(startCoord.getX() < 0 || startCoord.getX() >= sizeX){
        return pathFinal;
    }

    if(startCoord.getY() < 0 || startCoord.getY() >= sizeY){
        return pathFinal;
    }

    if(endCoord.getX() < 0 || endCoord.getX() >= sizeX){
        return pathFinal;
    }

    if(endCoord.getY() < 0 || endCoord.getY() >= sizeY){
        return pathFinal;
    }

    if(startCoord == endCoord){
        return pathFinal;
    }
    
    for(int y = 0; y < sizeY; ++y){
        for(int x = 0; x < sizeX; ++x ){
            maze[y][x].clear();

        }
    }

    vector<Tile*> openSet, closedSet;

    Tile *start = &maze[startCoord.getY()][startCoord.getX()], *end = &maze[endCoord.getY()][endCoord.getX()];

    vector<Tile*> endNeighbors;
    for(int i = 0; i < end->getCurrentNeighbors(); ++i){
        endNeighbors.push_back(end->getNeighbors(i));
    }
    openSet.push_back(start);

    while(openSet.size() > 0){
        int lowestIndex = 0;
        for(int i = 0; i < openSet.size(); ++i){
            if(openSet[i]->getF() < openSet[lowestIndex]->getF()){
                lowestIndex = i;
            }
        }

        Tile *current = openSet[lowestIndex];
        if(current == end){
          
            vector<Coord> temp, path;
            uint8_t cost = 0;
            while(current->getPrevious() != nullptr){
                cost += current->getCost();
                temp.push_back(Coord(current->getX(), current->getY()));
                current = current->getPrevious();
            }

            for(int i = temp.size()-1; i >= 0; --i){
                path.push_back(temp[i]);
            }

            pathFinal = Path(path);
            pathFinal.setCost(cost);
            pathFinal.setValid(true);
            return pathFinal;
        }

        //cout << "Before= " << openSet.size() << endl;
        closedSet.push_back(current);

        //cout << countsOnVector(closedSet, current) <<  endl;
        removeFromVector(openSet, current);
        //cout << "After= " << openSet.size() << endl;

       Tile* neighbors[4];
        int amountNeighbors = current->getCurrentNeighbors();
         for(int i = 0; i < amountNeighbors;++i){
            neighbors[i] = (current->getNeighbors(i));
        }

        for(int i = 0; i < amountNeighbors; ++i){
            Tile* neighbor = neighbors[i];
            if((neighbor->wasVisited() || neighbor == end) && !neighbor->isRamp()){
            ///MODIFIED ASTAR TO BE USED ON ROBOCUP
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
                        neighbor->setH(heuristic(*neighbor, *end));
                        neighbor->setF( neighbor->getG() + neighbor->getH());
                        neighbor->setPrevious(current);
                    }
                }
            }
        }
    }
    cout << "No solution" << endl;
    return pathFinal;
}
#include "Node.h"

Node::Node(){
    this->x = 0;
    this->y = 0;
}

Node::Node(int x, int y){
    f = 0;
    this->x = x;
    this->y = y;
}

void Node::setF(int f){
    this->f = f;
}

void Node::setH(int h){
    this->h = h;
}

void Node::setG(int g){
    this->g = g;
}

void Node::setX(int x){
    this->x = x;
}

void Node::setY(int y){
    this->y = y;
}

void Node::setPrevious(Node* previous){
    this->previous = previous;
}

void Node::setWall(Direction dir, bool wall){
    switch(dir){
        case Up:{
            isWallU = wall;
        }
        break;

        case Left:{
            isWallL = wall;
        }
        break;

        case Down:{
            isWallD = wall;
        }
        break;

        case Right:{
            isWallR = wall;
        }
        break;
    }
}

int Node::getF() const{
    return f;
}

int Node::getH() const{
    return h;
}

int Node::getG() const{
    return g;
}

int Node::getX() const{
    return x;
}

int Node::getY() const{
    return y;
}

bool Node::wallExists(Direction dir){
    switch(dir){
        case Up:{
            return isWallU;
        }
        break;

        case Left:{
            return isWallL;
        }
        break;

        case Down:{
            return isWallD;
        }
        break;

        case Right:{
            return isWallR;
        }
        break;
    }
}

Node* Node::getPrevious(){
    return previous;
}

void Node::addNeighbor(Node* neighbor){
    neighbors.push_back(neighbor);
}

vector<Node*>& Node::getNeighbors(){
    return neighbors;
}



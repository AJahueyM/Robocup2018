#include "Node.h"

Node::Node(){
    this->h = 0;
    this->f = 0;    
    this->g = 0;
}

Node::Node(int x, int y) : Coord(x, y){
    this->h = 0;
    this->f = 0;    
    this->g = 0;
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

void Node::setPrevious(Node* previous){
    this->previous = previous;
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

Node* Node::getPrevious(){
    return previous;
}

void Node::addNeighbor(Node* neighbor){
    if(currentNeighbors < 4){
        neighbors[currentNeighbors++] = neighbor;
    }
}

Node* Node::getNeighbors(int index){
    if(index < 4)
        return neighbors[index];
    else 
        return nullptr;
}

int Node::getCurrentNeighbors(){
    return currentNeighbors;
}



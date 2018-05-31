#include "Node.h"

Node::Node(){
    this->h = 0;
    this->f = 0;    
    this->g = 0;
}

Node::Node(uint8_t x, uint8_t y) : Coord(x, y){
    this->h = 0;
    this->f = 0;    
    this->g = 0;
}

void Node::setF(uint8_t f){
    this->f = f;
}

void Node::setH(uint8_t h){
    this->h = h;
}

void Node::setG(uint8_t g){
    this->g = g;
}

void Node::setPrevious(Node* previous){
    this->previous = previous;
}

uint8_t Node::getF() const{
    return f;
}

uint8_t Node::getH() const{
    return h;
}

uint8_t Node::getG() const{
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

Node* Node::getNeighbors(uint8_t index){
    if(index < 4)
        return neighbors[index];
    else 
        return nullptr;
}

uint8_t Node::getCurrentNeighbors(){
    return currentNeighbors;
}



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



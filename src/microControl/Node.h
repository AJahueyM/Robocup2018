#ifndef __NODE_H_
#define __NODE_H_
#include <ArduinoSTL.h>
#include "Utils.h"
using namespace std;

class Node{
public:
    Node();
    explicit Node(int x, int y);
    void setF(int f);
    void setH(int h);
    void setG(int g);
    void setX(int x);
    void setY(int y);
    void setPrevious(Node* previous);
    virtual void setWall(Direction dir, bool wall);
    int getF() const;
    int getH() const;
    int getG() const;
    int getX() const;
    int getY() const;
    virtual bool wallExists(Direction dir);
    Node* getPrevious();
    void addNeighbor(Node* neighbor);
    vector<Node*>& getNeighbors();

private:
    int f, h, g;
    int x, y;
    vector<Node*> neighbors;
    Node* previous = nullptr;
    bool isWallU = false, isWallL = false, isWallD = false, isWallR = false;
};
#endif
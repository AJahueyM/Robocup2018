#ifndef __NODE_H_
#define __NODE_H_
#include <ArduinoSTL.h>
#include "Utils.h"
#include "Coord.h"
using namespace std;

class Node : public Coord{
public:
    Node();
    explicit Node(int x, int y);
    void setF(int f);
    void setH(int h);
    void setG(int g);

    void setPrevious(Node* previous);
    virtual void setWall(Direction dir, bool wall) = 0;
    int getF() const;
    int getH() const;
    int getG() const;

    virtual bool wallExists(Direction dir) = 0;
    Node* getPrevious();
    void addNeighbor(Node* neighbor);
    Node* getNeighbors(int index);
    int getCurrentNeighbors();

private:
    int f, h, g;
    int currentNeighbors = 0;
    Node* neighbors[4];
    Node* previous = nullptr;
};
#endif
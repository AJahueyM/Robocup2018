#ifndef __NODE_H_
#define __NODE_H_
#include <ArduinoSTL.h>
#include "Utils.h"
#include "Coord.h"
using namespace std;

class Node : public Coord{
public:
    Node();
    explicit Node(uint8_t x, uint8_t y);
    void setF(uint8_t f);
    void setH(uint8_t h);
    void setG(uint8_t g);

    void setPrevious(Node* previous);
    virtual void setWall(Direction dir, bool wall) = 0;
    uint8_t getF() const;
    uint8_t getH() const;
    uint8_t getG() const;

    virtual bool wallExists(Direction dir) = 0;
    Node* getPrevious();
    void addNeighbor(Node* neighbor);
    Node* getNeighbors(uint8_t index);
    uint8_t getCurrentNeighbors();
    virtual uint8_t getCost() = 0;

private:
    uint8_t f, h, g;
    uint8_t currentNeighbors = 0;
    Node* neighbors[4];
    Node* previous = nullptr;
};
#endif
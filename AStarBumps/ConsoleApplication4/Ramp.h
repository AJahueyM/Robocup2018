#pragma once
#include "Node.h"
class Ramp {
public:
	Ramp(Node* origin, int floorTarget) { this->origin = origin, wasUsed = false; this->floorTarget = floorTarget; }
	
	void setUsed(bool wasUsed) { this->wasUsed = wasUsed; }
	bool getUsed() { return this->wasUsed; }

	void setOrigin(Node* node) { this->origin = node; }
	Node* getOrigin() { return this->origin; }

	void setEnd(Node* node) { this->end = node; }
	Node* getEnd() { return this->end; }

	int getFloorTarget() { return this->floorTarget; }
	void setFloorTarget(int floorTarget) { this->floorTarget = floorTarget; }
private:
	bool wasUsed;
	int floorTarget;
	Node* origin = nullptr, *end = nullptr;
};
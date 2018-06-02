#pragma once
#include "Node.h"
#include "Ramp.h"
#include <vector>

class Board {
public:
	Board() { ramps; map; hasRamp = false; origin = nullptr;};
	
	void setHasRamp(bool hasRamp) { this->hasRamp = hasRamp; }
	bool getHasRamp() { return this->hasRamp; }

	void setMap(vector<vector<Node>>& map) { this->map = map; }
	vector <vector<Node>>& getMap() { return this->map; }
	
	void addRamp(Ramp ramp) { ramps.emplace_back(ramp); }

	vector <Ramp>& getRamps() { return this->ramps; }

	Ramp* getRamp(int index) { return &ramps[index]; }
	void setRamp(Ramp ramp, int index) { this->ramps[index] = ramp; }

	Ramp* getRampAt(Node* node) {
		for (Ramp& ramp : ramps) {
			if (ramp.getOrigin() == node)
				return &ramp;
		}
		return nullptr;
	}
	bool hasNotUsedRamps() {
		for (Ramp& ramp : ramps) {
			if (!ramp.getUsed()) {
				return true;
			}
		}
		return false;
	}


private:
	vector <vector<Node>> map;
	vector <Ramp> ramps;
	bool hasRamp;
	Node* origin;

}; 
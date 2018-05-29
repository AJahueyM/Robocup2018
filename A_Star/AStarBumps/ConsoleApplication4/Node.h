#pragma once

enum BumpLevel {
	Small,
	Medium,
	Max,
	None
};

class Node {
public:
	Node(int x, int y) { this->x = x; this->y = y; g = 0; h = 0; f = 0; hasWallUp = false; hasWallDown = false; hasWallLeft = false; hasWallRigth = false; parent = nullptr; bumpType = None; };

	int getX() { return x; }
	void setX(int x) { this->x = x; }

	int getY() { return y; }
	void setY(int y) { this->y = y; }

	int getG() { return g; }
	void setG(int g) { this->g = g; }

	int getH() { return h; }
	void setH(int h) { this->h = h; }

	int getF() { return f; }
	void setF(int f) { this->f = f; }

	bool getHasWallUp() { return hasWallUp; }
	void setHasWallUp(bool hasWallUp) { this->hasWallUp = hasWallUp; }

	bool getHasWallDown() { return hasWallDown; }
	void setHasWallDown(bool hasWallDown) { this->hasWallDown = hasWallDown; }

	bool getHasWallLeft() { return hasWallLeft; }
	void setHasWallLeft(bool hasWallLeft) { this->hasWallLeft = hasWallLeft; }

	bool getHasWallRight() { return hasWallRigth; }
	void setHasWallRight(bool hasWallRigth) { this->hasWallRigth = hasWallRigth; }

	//bool getHasSmallBump() { return this->hasSmallBump; }
	//void setHasSamllBump(bool hasSmallBump) { this->hasSmallBump = hasSmallBump; }

	//bool gethasMediumBump() { return this->hasMediumBump; }
	//void sethasMediumBump(bool hasMediumBump) { this->hasMediumBump = hasMediumBump; }

	//bool gethasMaxBump() { return this->hasMaxBump; }
	//void sethasMaxBump(bool hasMaxBump) { this->hasMaxBump = hasMaxBump; }

	BumpLevel getBumpLevel() { return this->bumpType; };
	void setBumpLevel(BumpLevel bumpType) { this->bumpType = bumpType; };

	void addNeighbor(Node* node) { this->neighbors.emplace_back(node); }
	vector<Node*> getNeighbors() { return this->neighbors; }

	void setParent(Node* parent) { this->parent = parent; }
	Node* getParent() { return this->parent; }

private:
	int x, y, f, g, h;
	bool hasWallUp, hasWallDown, hasWallLeft, hasWallRigth; // hasSmallBump, hasMediumBump, hasMaxBump;
	BumpLevel bumpType;
	vector <Node*> neighbors;
	Node* parent;
};


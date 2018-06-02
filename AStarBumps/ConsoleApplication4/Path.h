#pragma once
class Path {
public:
	Path() {};
	Path(vector <Node*> path) { this->path = path; };
	Node* getCoordAt(int index) {if (index >= 0 && index < path.size()) { return path[index]; }};
	int getLength() { return path.size(); };
	vector <Node*> getPath() { return path; };

	void printPath(){cout << "X|Y" << endl; for (int i = path.size() - 1; i >= 0; --i) { cout << path[i]->getX() << " " << path[i]->getY() << endl; } cout << "CICLO COMPLETADO, DAR ENTER PARA CONTINUAR" << endl; }
	int getCost() { return this->cost; };
	void setCost(int cost) { this->cost = cost; };
private:
	vector<Node*> path;
	int cost;
};
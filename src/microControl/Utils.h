#ifndef __UTILS_H_
#define __UTILS_H_
#include <Arduino.h>
#include "Coord.h"
#include <ArduinoSTL.h>
using namespace std;

enum Direction {
	Up,
	Down,
	Left,
	Right
};

enum Color {
  White,
  Black,
  Silver
};

enum BumpLevel {
  Small,
  Medium,
  Max,
  None
};

float mapD(float x, float in_min, float in_max, float out_min, float out_max);

float shortestAngleTurn(float currentAngle, float target);

template<class T>
void removeFromVector(vector<T> &vector, T element){
    for(int j = vector.size() - 1; j >= 0; --j){
        if(vector[j] == element){
            vector.erase(vector.begin() + j);
        }
    }
}

template <class T>
int countsOnVector(vector<T> & vector, T element){
    int counter = 0;
    for(T el : vector){
        if(element == el)
            counter++;
    }

    return counter;
}
int calculateDistance(Coord a, Coord b) {
	int dx = abs(a.getX() - b.getX());
	int dy = abs(a.getY() - b.getY()); 
	return (dx + dy); 
}

Coord getClosestFrom(vector <Coord>& vec, Coord start) {
	Coord end;
	int distance = 0, minorDistance = 100;
	for (int i = 0; i < vec.size(); ++i) {
		Coord candidate = vec[i];
		distance = calculateDistance(start, candidate);
		if(minorDistance > distance) {
			minorDistance = distance;
			end = candidate;
		}
	}

	//cout << "LA TILE NO VISITADA MAS CERCANA (PROVENIENTE DE UNA TILE VISITADA) ES : " << end->getX() << " " << end->getY() << endl;
	return end;
}
#endif
